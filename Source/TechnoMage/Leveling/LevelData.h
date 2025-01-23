#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "TechnoMage/Enums/Enums.h"
#include "LevelData.generated.h"

USTRUCT(BlueprintType)
struct FLevelData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Leveling")
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Leveling")
	int32 RequiredXP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Leveling")
	int32 SkillPointsReward;
};

USTRUCT(BlueprintType)
struct FStatParams
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatParams")
	float BaseMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatParams")
	int IncreaseCostEvery = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatParams")
	int ExtraBonusEvery;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatParams")
	float ExtraBonusMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatParams")
	float MaxStatValue = 50;
};

USTRUCT(BlueprintType)
struct FCharacterParam
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatParams")
	float LevelMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatParams")
	float BaseValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatParams")
	ECharacterStatType MainParamMultiplierType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatParams")
	ECharacterStatType SecondParamMultiplierType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatParams")
	float SecondParamMultiplier;
};

UCLASS(Blueprintable, BlueprintType)
class TECHNOMAGE_API UStatsData : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats Multipliers")
	TMap<ECharacterStatType, FStatParams> StatMultipliers;

	// Конструктор по умолчанию
	UStatsData()
	{
	}
};

UCLASS(Blueprintable, BlueprintType)
class TECHNOMAGE_API UCharacterParamData : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats Multipliers")
	TMap<ECharacterParamType, FCharacterParam> CharacterParam;

	// Конструктор по умолчанию
	UCharacterParamData()
	{
	}
};
