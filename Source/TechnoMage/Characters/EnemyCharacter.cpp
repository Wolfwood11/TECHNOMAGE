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

void AEnemyCharacter::Die()
{
	// Получаем контроллер игрока
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("Die: PlayerController not found"));
		return;
	}

	// Получаем актёра игрока
	APawn* PlayerPawn = PlayerController->GetPawn();
	if (!PlayerPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("Die: PlayerPawn not found"));
		return;
	}

	// Проверяем, реализует ли актёр интерфейс ICharacterGetersInterface
	if (PlayerPawn->GetClass()->ImplementsInterface(UCharacterSettersInterface::StaticClass()))
	{
		// Вызываем метод AddExp у игрока
		ICharacterSettersInterface::Execute_AddExp(PlayerPawn, Exp); // Добавляем 100 опыта
		UE_LOG(LogTemp, Log, TEXT("Die: Added 100 exp to player"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Die: PlayerPawn does not implement ICharacterGetersInterface"));
	}
	Super::Die();
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
