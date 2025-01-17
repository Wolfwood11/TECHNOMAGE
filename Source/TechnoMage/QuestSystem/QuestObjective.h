#pragma once

#include "CoreMinimal.h"
#include "QuestTargetType.h"
#include "QuestObjective.generated.h"

// Структура для описания целей квеста
USTRUCT(BlueprintType)
struct FQuestObjective
{
	GENERATED_BODY()

	// Тип цели
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
	EQuestTargetType TargetType;

	// Описание цели
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
	FString Description;

	// Текущий прогресс
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
	int32 CurrentCount;

	// Необходимый прогресс
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
	int32 TargetCount;

	// Дополнительные данные для цели (например, тип врага, ID предмета или область)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
	FString TargetData;

	FQuestObjective()
		: TargetType(EQuestTargetType::CollectItems), Description(TEXT("")), CurrentCount(0), TargetCount(1), TargetData(TEXT("")) {}

	// Проверка соответствия целевых данных
	bool Matches(EQuestTargetType InTargetType, const FString& InTargetData) const
	{
		return TargetType == InTargetType && (TargetData.IsEmpty() || TargetData == InTargetData);
	}
};
