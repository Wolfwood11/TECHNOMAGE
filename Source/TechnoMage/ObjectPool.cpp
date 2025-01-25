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

		TArray<TObjectPtr<ABasePoolableActor>>& Pool = ObjectPools.FindOrAdd(ActorClass);
		for (int32 i = 0; i < InitialCount; ++i)
		{
			if (TObjectPtr<ABasePoolableActor> NewObject = CreateNewObject(ActorClass))
			{
				Pool.Add(NewObject);
			}
		}
	}
}

TObjectPtr<ABasePoolableActor> AObjectPool::CreateNewObject(const TSubclassOf<ABasePoolableActor>& Type)
{
	if (!Type || !Type->IsChildOf(ABasePoolableActor::StaticClass())) return nullptr;

	TObjectPtr<ABasePoolableActor> NewObject = GetWorld()->SpawnActor<ABasePoolableActor>(Type, FVector::ZeroVector, FRotator::ZeroRotator);
	if (NewObject)
	{
		NewObject->ReturnToPool();
	}

	return NewObject;
}

TObjectPtr<ABasePoolableActor> AObjectPool::GetObject(const TSubclassOf<ABasePoolableActor>& Type)
{
	if (!Type || !Type->IsChildOf(ABasePoolableActor::StaticClass())) return nullptr;

	// Проверяем, есть ли объекты в пуле для данного типа
	TArray<TObjectPtr<ABasePoolableActor>>* Pool = ObjectPools.Find(Type);
	if (Pool && !Pool->IsEmpty())
	{
		TObjectPtr<ABasePoolableActor>* FoundObject = Pool->FindByPredicate([](const TObjectPtr<ABasePoolableActor>& Object)
			{
				return Object && Object->IsPooled();
			});

		if (FoundObject)
		{
			(*FoundObject)->GetFromPool();
			return *FoundObject;
		}
	}

	// Если объект не найден в пуле, создаём новый
	TObjectPtr<ABasePoolableActor> NewObject = CreateNewObject(Type);
	if (NewObject)
	{
		if (!Pool)
		{
			ObjectPools.Add(Type, TArray<TObjectPtr<ABasePoolableActor>>());
		}
		ObjectPools[Type].Add(NewObject);
		NewObject->GetFromPool();
		return NewObject;
	}

	return nullptr; // Не удалось найти или создать объект
}

void AObjectPool::ReturnObject(const TObjectPtr<ABasePoolableActor>& Object)
{
	if (Object)
	{
		Object->ReturnToPool();
	}
}
