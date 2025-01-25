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

	if (!PlayerActor.IsValid() || !PlayerActor.Get())
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

void AEnemyAIController::MoveToPlayer(TWeakObjectPtr<AActor> actor)
{
	if (actor.IsValid() && actor.Get())
	{
		MoveToActor(PlayerActor.Get(), 5.f); // Идём к игроку с минимальным расстоянием 5 юнитов
	}
}
