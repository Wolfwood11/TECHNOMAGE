#pragma once

#include "CoreMinimal.h"
#include "QuestTargetType.generated.h"

// Типы целей квестов
UENUM(BlueprintType)
enum class EQuestTargetType : uint8
{
	CollectItems UMETA(DisplayName = "Collect Items"), // Собрать предметы
	TalkToNPC UMETA(DisplayName = "Talk to NPC"),     // Поговорить с NPC
	ReachArea UMETA(DisplayName = "Reach Area"),      // Достичь области
	KillEnemies UMETA(DisplayName = "Kill Enemies")   // Убить врагов
};
