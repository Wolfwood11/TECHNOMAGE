#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

UCLASS()
class TECHNOMAGE_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	// Логика обновления
	virtual void Tick(float DeltaTime) override;

	// Перемещение к игроку
	void MoveToPlayer(AActor* actor);

private:
	// Ссылка на игрока
	UPROPERTY()
	AActor* PlayerActor;

	// Радиус атаки
	UPROPERTY(EditAnywhere, Category = "AI")
	float AttackRadius = 200.f;
};
