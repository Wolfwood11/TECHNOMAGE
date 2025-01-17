#pragma once

#include "CoreMinimal.h"
#include "EnemySaveData.generated.h"

// Структура для данных врагов
USTRUCT(BlueprintType)
struct FEnemySaveData
{
	GENERATED_BODY()

	// Уникальный идентификатор врага
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enemy")
	FString EnemyID;

	// Позиция врага
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enemy")
	FVector EnemyLocation;

	// Уровень здоровья врага
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enemy")
	float Health;

	FEnemySaveData()
		: EnemyID(TEXT("")), EnemyLocation(FVector::ZeroVector), Health(100.0f)
	{
	}
};
