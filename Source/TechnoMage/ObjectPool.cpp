#include "ObjectPool.h"
#include "BasePoolableActor.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Subsystems/ActorTrackingSubsystem.h"

AObjectPool::AObjectPool()
{
	PrimaryActorTick.bCanEverTick = false;
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
	for (const TPair<TSubclassOf<ABasePoolableActor>, int32>& Entry : PoolConfig)
	{
		TSubclassOf<ABasePoolableActor> ActorClass = Entry.Key;
		int32 InitialCount = Entry.Value;

		if (!ActorClass || InitialCount <= 0) continue;

		TArray<ABasePoolableActor*>& Pool = ObjectPools.FindOrAdd(ActorClass);
		for (int32 i = 0; i < InitialCount; ++i)
		{
			ABasePoolableActor* NewObject = CreateNewObject(ActorClass);
			if (NewObject)
			{
				Pool.Add(NewObject);
			}
		}
	}
}

ABasePoolableActor* AObjectPool::CreateNewObject(const TSubclassOf<ABasePoolableActor>& Type)
{
	if (!Type || !Type->IsChildOf(ABasePoolableActor::StaticClass())) return nullptr;

	ABasePoolableActor* NewObject = GetWorld()->SpawnActor<ABasePoolableActor>(Type, FVector::ZeroVector, FRotator::ZeroRotator);
	if (NewObject)
	{
		NewObject->ReturnToPool();
	}

	return NewObject;
}

ABasePoolableActor* AObjectPool::GetObject(const TSubclassOf<ABasePoolableActor>& Type)
{
	if (!Type || !Type->IsChildOf(ABasePoolableActor::StaticClass())) return nullptr;

	// Проверяем, есть ли объекты в пуле для данного типа
	TArray<ABasePoolableActor*>* Pool = ObjectPools.Find(Type);
	if (Pool)
	{
		for (ABasePoolableActor* Object : *Pool)
		{
			if (Object && Object->IsPooled())
			{
				Object->GetFromPool();
				return Object;
			}
		}
	}

	// Если объект не найден в пуле, создаём новый
	ABasePoolableActor* NewObject = CreateNewObject(Type);
	if (NewObject)
	{
		if (!Pool)
		{
			ObjectPools.Add(Type, TArray<ABasePoolableActor*>());
		}
		ObjectPools[Type].Add(NewObject);
		NewObject->GetFromPool();
		return NewObject;
	}

	return nullptr; // Не удалось найти или создать объект
}

void AObjectPool::ReturnObject(ABasePoolableActor* Object)
{
	if (Object)
	{
		Object->ReturnToPool();
	}
}
