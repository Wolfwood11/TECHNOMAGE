#pragma once

#include "CoreMinimal.h"
#include "Quest.h"
#include "QuestObjective.h"
#include "QuestSaveData.generated.h"

// Структура для хранения данных о состоянии квеста
USTRUCT(BlueprintType)
struct FQuestSaveData
{
	GENERATED_BODY()

	// Уникальное имя квеста
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestSave")
	FString QuestName;

	// Текущее состояние квеста
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestSave")
	EQuestState QuestState;

	// Прогресс по целям
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestSave")
	TArray<int32> ObjectivesProgress;

	FQuestSaveData()
		: QuestName(TEXT("")), QuestState(EQuestState::NotStarted) {}
};
