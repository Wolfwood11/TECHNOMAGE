#pragma once

#include "CoreMinimal.h"
#include "QuestObjective.h"
#include "QuestChain.h"
#include "Quest.generated.h"

// Состояние квеста
UENUM(BlueprintType)
enum class EQuestState : uint8
{
	NotStarted UMETA(DisplayName = "Not Started"),
	InProgress UMETA(DisplayName = "In Progress"),
	Completed UMETA(DisplayName = "Completed"),
	Failed UMETA(DisplayName = "Failed")
};

// Основной класс для квеста
UCLASS(BlueprintType)
class TECHNOMAGE_API UQuest : public UDataAsset
{
	GENERATED_BODY()

public:
	// Название квеста
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	FString QuestName;

	// Описание квеста
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	FString Description;

	// Цели квеста
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	TArray<FQuestObjective> Objectives;

	// Текущее состояние квеста
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	EQuestState QuestState;

	// Награды за выполнение
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	TMap<FString, int32> Rewards;

	// Последствия выполнения (следующие квесты)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	TMap<EQuestState, FQuestChain> NextQuests;

	UQuest()
		: QuestName(TEXT("")), Description(TEXT("")), QuestState(EQuestState::NotStarted) {}

	// Проверка завершения всех целей
	UFUNCTION(BlueprintCallable, Category = "Quest")
	bool AreObjectivesCompleted() const;

	// Обновление прогресса по цели
	UFUNCTION(BlueprintCallable, Category = "Quest")
	void UpdateObjective(int32 ObjectiveIndex, int32 Increment);

	// Получение следующего квеста по результату
	UFUNCTION(BlueprintCallable, Category = "Quest")
	TArray<UQuest*> GetNextQuests(EQuestState ResultState) const;
};
