#include "ObjectPool.h"
#include "BasePoolableActor.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Subsystems/ActorTrackingSubsystem.h"

AObjectPool::AObjectPool()
{
	PrimaryActorTick.bCanEverTick = false;
	InitialPoolSize = 5; // Базовый размер пула по умолчанию
}

void AObjectPool::BeginPlay()
{
	Super::BeginPlay();
	if (UActorTrackingSubsystem* Subsystem = GetWorld()->GetSubsystem<UActorTrackingSubsystem>())
	{
		Subsystem->RegisterActor(this);
	}
	InitializePool();
}

void AObjectPool::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UActorTrackingSubsystem* Subsystem = GetWorld()->GetSubsystem<UActorTrackingSubsystem>())
	{
		Subsystem->UnregisterActor(this);
	}
	Super::EndPlay(EndPlayReason);
}

void AObjectPool::InitializePool()
{
	for (const TPair<int32, TSubclassOf<ABasePoolableActor>>& Entry : PoolConfig)
	{
		int32 Key = Entry.Key;
		TSubclassOf<ABasePoolableActor> ActorClass = Entry.Value;

		if (!ActorClass) continue;

		TArray<ABasePoolableActor*>& Pool = ObjectPools.FindOrAdd(Key);
		for (int32 i = 0; i < InitialPoolSize; ++i)
		{
			ABasePoolableActor* NewObject = CreateNewObject(Key);
			if (NewObject)
			{
				Pool.Add(NewObject);
			}
		}
	}
}

ABasePoolableActor* AObjectPool::CreateNewObject(int32 Key)
{
	if (!PoolConfig.Contains(Key)) return nullptr;

	TSubclassOf<ABasePoolableActor> ActorClass = PoolConfig[Key];
	if (!ActorClass) return nullptr;

	ABasePoolableActor* NewObject = GetWorld()->SpawnActor<ABasePoolableActor>(ActorClass, FVector::ZeroVector, FRotator::ZeroRotator);
	if (NewObject)
	{
		_actors.Add(NewObject);
		NewObject->ReturnToPool();
	}

	return NewObject;
}

ABasePoolableActor* AObjectPool::GetObject(int32 Key)
{
	if (!ObjectPools.IsEmpty() && ObjectPools.Contains(Key))
	{
		TArray<ABasePoolableActor*>& Pool = ObjectPools[Key];
		for (ABasePoolableActor* Object : Pool)
		{
			if (Object && Object->IsPooled())
			{
				Object->GetFromPool();
				return Object;
			}
		}

		// Если в пуле нет доступных объектов, создаём новый
		ABasePoolableActor* NewObject = CreateNewObject(Key);
		if (NewObject)
		{
			Pool.Add(NewObject);
			NewObject->GetFromPool();
			return NewObject;
		}
	}

	return nullptr; // Нет доступных объектов и не удалось создать новый
}

class ABasePoolableActor* AObjectPool::GetObject(const TSubclassOf<class ABasePoolableActor>& Type)
{
	// Проверяем, есть ли объекты в пуле и соответствуют ли они указанному типу
	for (auto& PoolPair : ObjectPools)
	{
		TArray<ABasePoolableActor*>& Pool = PoolPair.Value;

		for (ABasePoolableActor* Object : Pool)
		{
			if (Object && Object->IsPooled() && Object->IsA(Type))
			{
				Object->GetFromPool();
				return Object;
			}
		}
	}

	// Если нет доступных объектов, создаём новый объект указанного типа
	if (*Type)
	{
		ABasePoolableActor* NewObject = GetWorld()->SpawnActor<ABasePoolableActor>(Type);
		if (NewObject)
		{
			int32 Key = ObjectPools.Num(); // Генерируем новый ключ
			ObjectPools.FindOrAdd(Key).Add(NewObject);
			NewObject->GetFromPool();
			return NewObject;
		}
	}

	return nullptr; // Нет доступных объектов и не удалось создать новый
}

void AObjectPool::ReturnObject(ABasePoolableActor* Object)
{
	if (Object)
	{
		Object->ReturnToPool();
	}
}
