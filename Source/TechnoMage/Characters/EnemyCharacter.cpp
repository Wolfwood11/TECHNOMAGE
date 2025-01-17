#include "EnemyCharacter.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TechnoMage/AI/EnemyAIController.h"
#include "TechnoMage/Components/CharacterResourcePool.h"

AEnemyCharacter::AEnemyCharacter()
{
	AIControllerClass = AEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	if (HealthPool)
	{
		HealthPool->SetAllowNegativeResource(true);
		HealthPool->SetRegenRate(0);
		HealthPool->SetComponentTickEnabled(false);
	}
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	ConfigureMovement();
}

void AEnemyCharacter::ConfigureMovement() const
{
	if (UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
	{
		MovementComponent->bUseRVOAvoidance = true;
		MovementComponent->AvoidanceConsiderationRadius = 100.f;
		MovementComponent->AvoidanceWeight = 0.5f;
	}
}

void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
