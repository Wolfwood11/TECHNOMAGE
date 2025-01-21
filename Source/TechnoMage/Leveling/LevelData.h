#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "LevelData.generated.h"

USTRUCT(BlueprintType)
struct FLevelData : public FTableRowBase
{
	GENERATED_BODY()

public:
	// Уровень
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	int32 Level;

	// Необходимый опыт для этого уровня
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	int32 RequiredXP;

	// Бонус здоровья при достижении уровня
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	float HealthBonus;

	// Бонус маны при достижении уровня
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	float ManaBonus;

	// Количество скилл-поинтов за уровень
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	int32 SkillPointsReward;
};
