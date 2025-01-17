#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "TechnoMage/QuestSystem/Quest.h"
#include "TechnoMage/QuestSystem/QuestSaveData.h"
#include "QuestSubsystem.generated.h"

UCLASS()
class TECHNOMAGE_API UQuestSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// Добавить новый квест
	UFUNCTION(BlueprintCallable, Category = "QuestSystem")
	void AddQuest(UQuest* Quest);

	// Обновить прогресс по конкретной цели (по типу цели и данным)
	UFUNCTION(BlueprintCallable, Category = "QuestSystem")
	void UpdateQuestProgress(EQuestTargetType TargetType, const FString& TargetData, int32 Count);

	// Завершить квест
	UFUNCTION(BlueprintCallable, Category = "QuestSystem")
	void CompleteQuest(const FString& QuestName);

	// Сохранить состояние квестов
	UFUNCTION(BlueprintCallable, Category = "QuestSystem")
	void SaveQuestStates();

	// Загрузить состояние квестов
	UFUNCTION(BlueprintCallable, Category = "QuestSystem")
	void LoadQuestStates();

private:
	// Список всех активных квестов
	UPROPERTY()
	TArray<UQuest*> ActiveQuests;

	// Список завершённых квестов
	UPROPERTY()
	TArray<UQuest*> CompletedQuests;

	// Найти квест по имени
	UQuest* FindQuestByName(const FString& QuestName) const;

	// Сохранённые данные о квестах
	UPROPERTY()
	TArray<FQuestSaveData> SavedQuestStates;
};
