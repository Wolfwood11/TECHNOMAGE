#pragma once

#include "CoreMinimal.h"
#include "QuestChain.generated.h"

class UQuest;

USTRUCT(BlueprintType)
struct FQuestChain
{
	GENERATED_BODY()

	// Массив квестов
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestChain")
	TArray<UQuest*> Quests;

	FQuestChain() {}
};
