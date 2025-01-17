#include "FlyingSpell.h"
#include "GameFramework/ProjectileMovementComponent.h"

AFlyingSpell::AFlyingSpell()
{
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 2000.0f;
	ProjectileMovement->MaxSpeed = 2000.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.0f;
	ProjectileMovement->bForceSubStepping = true;
}

void AFlyingSpell::BeginPlay()
{
	Super::BeginPlay();
}

void AFlyingSpell::UpdateComponentsState(bool value)
{
	Super::UpdateComponentsState(value);
	ProjectileMovement->SetComponentTickEnabled(value);
}

void AFlyingSpell::TriggerEffect(const FHitResult& ImpactResult)
{
	Super::TriggerEffect(ImpactResult);
	if (ProjectileMovement)
	{
		ProjectileMovement->StopMovementImmediately();
	}
	ReturnToPool();
	// Дополнительная логика для FlyingSpell, если потребуется
}

void AFlyingSpell::ActivateFromPoll(const FTransform& transform, AActor* NewInstigatorActor)
{
	Super::ActivateFromPoll(transform, NewInstigatorActor);
	if (ProjectileMovement)
	{
		ProjectileMovement->Velocity = GetActorRotation().Vector() * ProjectileMovement->InitialSpeed;
	}
}

void AFlyingSpell::ReturnToPool()
{
	Super::ReturnToPool();
	if (ProjectileMovement)
	{
		ProjectileMovement->StopMovementImmediately();
	}
}
