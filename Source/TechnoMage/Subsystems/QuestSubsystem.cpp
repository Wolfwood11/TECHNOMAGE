#include "QuestSubsystem.h"

#include "TechnoMage/QuestSystem/Quest.h"
#include "TechnoMage/QuestSystem/QuestSaveData.h"

void UQuestSubsystem::AddQuest(UQuest* Quest)
{
	if (Quest && !ActiveQuests.Contains(Quest))
	{
		ActiveQuests.Add(Quest);
		Quest->QuestState = EQuestState::InProgress;
	}
}

void UQuestSubsystem::UpdateQuestProgress(EQuestTargetType TargetType, const FString& TargetData, int32 Count)
{
	for (UQuest* Quest : ActiveQuests)
	{
		if (!Quest || Quest->QuestState != EQuestState::InProgress)
		{
			continue;
		}

		for (int32 i = 0; i < Quest->Objectives.Num(); i++)
		{
			FQuestObjective& Objective = Quest->Objectives[i];

			// Проверяем, соответствует ли цель типу и данным
			if (Objective.Matches(TargetType, TargetData))
			{
				Objective.CurrentCount = FMath::Clamp(Objective.CurrentCount + Count, 0, Objective.TargetCount);

				// Если цель выполнена, проверяем завершение квеста
				if (Objective.CurrentCount >= Objective.TargetCount && Quest->AreObjectivesCompleted())
				{
					CompleteQuest(Quest->QuestName);
				}
			}
		}
	}

	// Сохраняем прогресс
	SaveQuestStates();
}

void UQuestSubsystem::CompleteQuest(const FString& QuestName)
{
	UQuest* Quest = FindQuestByName(QuestName);
	if (Quest && Quest->AreObjectivesCompleted())
	{
		Quest->QuestState = EQuestState::Completed;
		ActiveQuests.Remove(Quest);
		CompletedQuests.Add(Quest);

		// Активируем следующие квесты
		TArray<UQuest*> NextQuests = Quest->GetNextQuests(EQuestState::Completed);
		for (UQuest* NextQuest : NextQuests)
		{
			AddQuest(NextQuest);
		}

		// Сохраняем состояние
		SaveQuestStates();
	}
}

UQuest* UQuestSubsystem::FindQuestByName(const FString& QuestName) const
{
	for (UQuest* Quest : ActiveQuests)
	{
		if (Quest && Quest->QuestName == QuestName)
		{
			return Quest;
		}
	}

	for (UQuest* Quest : CompletedQuests)
	{
		if (Quest && Quest->QuestName == QuestName)
		{
			return Quest;
		}
	}

	return nullptr;
}

void UQuestSubsystem::SaveQuestStates()
{
	SavedQuestStates.Empty();

	// Сохраняем состояние всех активных квестов
	for (UQuest* Quest : ActiveQuests)
	{
		FQuestSaveData SaveData;
		SaveData.QuestName = Quest->QuestName;
		SaveData.QuestState = Quest->QuestState;

		for (const FQuestObjective& Objective : Quest->Objectives)
		{
			SaveData.ObjectivesProgress.Add(Objective.CurrentCount);
		}

		SavedQuestStates.Add(SaveData);
	}

	// Сохраняем состояние завершённых квестов
	for (UQuest* Quest : CompletedQuests)
	{
		FQuestSaveData SaveData;
		SaveData.QuestName = Quest->QuestName;
		SaveData.QuestState = Quest->QuestState;

		SavedQuestStates.Add(SaveData);
	}

	UE_LOG(LogTemp, Log, TEXT("Quest states saved."));
}

void UQuestSubsystem::LoadQuestStates()
{
	for (const FQuestSaveData& SaveData : SavedQuestStates)
	{
		UQuest* Quest = FindQuestByName(SaveData.QuestName);
		if (!Quest)
		{
			Quest = NewObject<UQuest>();
			Quest->QuestName = SaveData.QuestName;
			Quest->QuestState = SaveData.QuestState;

			// Загружаем прогресс по целям
			for (int32 i = 0; i < SaveData.ObjectivesProgress.Num(); i++)
			{
				if (i < Quest->Objectives.Num())
				{
					Quest->Objectives[i].CurrentCount = SaveData.ObjectivesProgress[i];
				}
			}

			// Добавляем квест в активные или завершённые
			if (SaveData.QuestState == EQuestState::Completed)
			{
				CompletedQuests.Add(Quest);
			}
			else
			{
				ActiveQuests.Add(Quest);
			}
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Quest states loaded."));
}
