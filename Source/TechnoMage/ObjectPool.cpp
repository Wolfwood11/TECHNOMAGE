#include "ObjectPool.h"
#include "BasePoolableActor.h"
#include "Engine/World.h"
#include "TimerManager.h"

AObjectPool::AObjectPool()
{
	PrimaryActorTick.bCanEverTick = false;
	InitialPoolSize = 5; // Базовый размер пула по умолчанию
}

void AObjectPool::BeginPlay()
{
	Super::BeginPlay();
	InitializePool();
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

void AObjectPool::ReturnObject(ABasePoolableActor* Object)
{
	if (Object)
	{
		Object->ReturnToPool();
	}
}
