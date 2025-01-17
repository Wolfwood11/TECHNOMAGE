#pragma once

#include "CoreMinimal.h"
#include "WorldSaveData.generated.h"

// Структура для данных мира
USTRUCT(BlueprintType)
struct FWorldSaveData
{
	GENERATED_BODY()

	// Время в игре
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "World")
	float GameTime;

	// Текущее состояние квестов
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "World")
	TMap<FString, bool> QuestStates;

	FWorldSaveData()
		: GameTime(0.0f)
	{
	}
};
