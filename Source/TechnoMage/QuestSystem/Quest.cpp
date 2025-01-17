#include "Quest.h"

bool UQuest::AreObjectivesCompleted() const
{
	for (const FQuestObjective& Objective : Objectives)
	{
		if (Objective.CurrentCount < Objective.TargetCount)
		{
			return false; // Если хотя бы одна цель не выполнена, квест не завершён
		}
	}
	return true;
}

TArray<UQuest*> UQuest::GetNextQuests(EQuestState ResultState) const
{
	if (NextQuests.Contains(ResultState))
	{
		return NextQuests[ResultState].Quests;
	}
	return {};
}

void UQuest::UpdateObjective(int32 ObjectiveIndex, int32 Increment)
{
	if (ObjectiveIndex >= 0 && ObjectiveIndex < Objectives.Num())
	{
		FQuestObjective& Objective = Objectives[ObjectiveIndex];
		Objective.CurrentCount = FMath::Clamp(Objective.CurrentCount + Increment, 0, Objective.TargetCount);

		// Если все цели выполнены, завершаем квест
		if (AreObjectivesCompleted())
		{
			QuestState = EQuestState::Completed;
		}
	}
}
