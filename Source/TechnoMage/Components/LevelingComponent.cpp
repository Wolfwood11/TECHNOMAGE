#include "LevelingComponent.h"

#include "Serialization/BufferArchive.h"
#include "TechnoMage/Interfaces/CharacterEventsInterface.h"
#include "TechnoMage/Leveling/LevelData.h"

ULevelingComponent::ULevelingComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	CurrentLevel = 1;
	CurrentExperience = 0;
	SkillPoints = 0;
}

void ULevelingComponent::SaveData_Implementation(TArray<uint8>& OutData)
{
	FBufferArchive Archive;
	Archive << CurrentLevel;
	Archive << CurrentExperience;
	Archive << SkillPoints;
	Archive << AllocatedStats;

	OutData = static_cast<TArray<uint8>>(Archive);
}

void ULevelingComponent::LoadData_Implementation(const TArray<uint8>& InData)
{
	FMemoryReader Reader(InData);
	Reader << CurrentLevel;
	Reader << CurrentExperience;
	Reader << SkillPoints;
	Reader << AllocatedStats;
}

void ULevelingComponent::BeginPlay()
{
	Super::BeginPlay();
}

void ULevelingComponent::AddExperience(int32 Amount)
{
	CurrentExperience += Amount;
	CheckForLevelUp();
}

void ULevelingComponent::CheckForLevelUp()
{
	while (LevelDataTable)
	{
		const auto* LevelData = GetLevelData(CurrentLevel);
		if (!LevelData || CurrentExperience < LevelData->RequiredXP)
		{
			break;
		}

		CurrentExperience -= LevelData->RequiredXP;
		CurrentLevel++;
		SkillPoints += LevelData->SkillPointsReward;

		OnLevelUp.Broadcast(CurrentLevel);

		if (!GetOwner() || !GetOwner()->GetClass()->ImplementsInterface(UCharacterEventsInterface::StaticClass()))
		{
			return;
		}

		ICharacterEventsInterface::Execute_OnLevelUp(GetOwner(), CurrentLevel);
	}
}

int32 ULevelingComponent::GetLevel() const
{
	return CurrentLevel;
}

int32 ULevelingComponent::GetExperience() const
{
	return CurrentExperience;
}

int32 ULevelingComponent::GetExperienceToNextLevel() const
{
	if (const auto* LevelData = GetLevelData(CurrentLevel))
	{
		return LevelData->RequiredXP - CurrentExperience;
	}
	return 0;
}

int32 ULevelingComponent::GetAvailableStatPoints() const
{
	return SkillPoints;
}

int32 ULevelingComponent::GetStatValue(ECharacterStatType StatType) const
{
	return AllocatedStats.FindRef(StatType);
}

int32 ULevelingComponent::GetStatUpgradeCost(ECharacterStatType StatType) const
{
	int32 Allocated = AllocatedStats.FindRef(StatType);
	const FStatParams* Params = StatsData->StatMultipliers.Find(StatType);
	if (!Params)
	{
		return 1; // Default cost
	}

	return 1 + Allocated / Params->IncreaseCostEvery;
}

bool ULevelingComponent::AllocateStatPoint(ECharacterStatType StatType)
{
	if (SkillPoints <= 0 || !StatsData)
	{
		return false;
	}

	const FStatParams* StatParams = StatsData->StatMultipliers.Find(StatType);
	if (!StatParams)
	{
		return false;
	}

	int32& Allocated = AllocatedStats.FindOrAdd(StatType);
	if (Allocated >= StatParams->MaxStatValue)
	{
		return false;
	}

	int32 Cost = 1 + Allocated / StatParams->IncreaseCostEvery;

	if (SkillPoints >= Cost)
	{
		Allocated++;
		SkillPoints -= Cost;
		return true;
	}

	return false;
}

float ULevelingComponent::GetStatMultiplier(ECharacterStatType StatType) const
{
	const FStatParams* StatParams = StatsData ? StatsData->StatMultipliers.Find(StatType) : nullptr;
	if (!StatParams)
	{
		return 0.f;
	}

	int32 Allocated = AllocatedStats.Contains(StatType) ? AllocatedStats[StatType] : 0;

	return StatParams->BaseMultiplier * Allocated +
		StatParams->ExtraBonusMultiplier * (Allocated / StatParams->ExtraBonusEvery);
}

void ULevelingComponent::SetStatsData(UStatsData* NewStatsData)
{
	StatsData = NewStatsData;
}

const FLevelData* ULevelingComponent::GetLevelData(int32 Level) const
{
	if (!LevelDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("LevelDataTable is null"));
		return nullptr;
	}

	// Перебираем все строки в DataTable
	for (const auto& RowPair : LevelDataTable->GetRowMap())
	{
		// Приводим данные строки к типу FLevelData
		const FLevelData* RowData = reinterpret_cast<const FLevelData*>(RowPair.Value);
		if (RowData && RowData->Level == Level)
		{
			return RowData; // Если уровень совпал, возвращаем строку
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Level %d not found in LevelDataTable"), Level);
	return nullptr;
}

float ULevelingComponent::GetCharacterParam(ECharacterParamType ParamType) const
{
	// Получаем данные для параметра
	const FCharacterParam* ParamData = CharacterParamData ? CharacterParamData->CharacterParam.Find(ParamType) : nullptr;
	if (!ParamData)
	{
		// Если параметр не найден, возвращаем базовое значение по умолчанию
		return 0.0f;
	}

	// Рассчитываем основное влияние
	float MainMultiplier = GetStatMultiplier(ParamData->MainParamMultiplierType);

	// Рассчитываем вторичное влияние
	float SecondaryMultiplier = GetStatMultiplier(ParamData->SecondParamMultiplierType) * ParamData->SecondParamMultiplier;

	float LevelMultiplier = ParamData->LevelMultiplier * GetLevel();

	// Суммируем базовое значение, основное влияние и вторичное влияние
	return 1.f + LevelMultiplier + ParamData->BaseValue * (MainMultiplier + SecondaryMultiplier);
}
