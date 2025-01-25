#include "BasePoolableActor.h"
#include "ObjectPool.h"

ABasePoolableActor::ABasePoolableActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

bool ABasePoolableActor::IsPooled() const
{
	return isPooled;
}

void ABasePoolableActor::UpdateComponentsState(bool value)
{
	Super::SetActorTickEnabled(value);
}

void ABasePoolableActor::GetFromPool()
{
	isPooled = false;
}

void ABasePoolableActor::ReturnToPool()
{
	isPooled = true;
	InstigatorActor = nullptr; // Сбрасываем ссылку на инициатора
	UpdateComponentsState(false);
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);

}

AActor* ABasePoolableActor::GetInstigatorActor() const
{
	if (InstigatorActor.IsValid())
	{
		return InstigatorActor.Get();
	}
	return nullptr;
}

void ABasePoolableActor::ActivateFromPoll(const FTransform& transform, TWeakObjectPtr<AActor> NewInstigatorActor)
{
	CurrentActiveLifeTime = 0.f;
	InstigatorActor = NewInstigatorActor;
	UpdateComponentsState(true);
	SetActorTransform(transform, false, nullptr, ETeleportType::TeleportPhysics);
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
}

void ABasePoolableActor::Tick(float DeltaSeconds)
{
	if (isPooled)
	{
		return;
	}

	CurrentActiveLifeTime += DeltaSeconds;
	if (CurrentActiveLifeTime >= ActiveLifeTime)
	{
		ReturnToPool();
	}

	Super::Tick(DeltaSeconds);
}
