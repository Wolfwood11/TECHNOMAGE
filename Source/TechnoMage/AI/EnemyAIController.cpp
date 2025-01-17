#include "EnemyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	// Найти игрока
	PlayerActor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

void AEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!PlayerActor)
	{
		return;
	}

	// Расстояние до игрока
	float DistanceToPlayer = FVector::Dist(GetPawn()->GetActorLocation(), PlayerActor->GetActorLocation());

	if (DistanceToPlayer > AttackRadius)
	{
		// Если игрок вне радиуса атаки, идём к нему
		MoveToPlayer(PlayerActor);
	}
	else
	{
		// Если игрок в радиусе атаки, останавливаемся
		StopMovement();
	}
}

void AEnemyAIController::MoveToPlayer(AActor* actor)
{
	if (actor)
	{
		MoveToActor(PlayerActor, 5.f); // Идём к игроку с минимальным расстоянием 5 юнитов
	}
}
