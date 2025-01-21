#include "LevelingComponent.h"

#include "Serialization/BufferArchive.h"
#include "TechnoMage/Leveling/LevelData.h"

ULevelingComponent::ULevelingComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	CurrentLevel = 1;
	CurrentExperience = 0;
	SkillPoints = 0;
	LevelDataTable = nullptr;
}

void ULevelingComponent::SaveData_Implementation(TArray<uint8>& OutData)
{
	FBufferArchive Archive;

	// Сохраняем данные
	Archive << CurrentLevel;
	Archive << CurrentExperience;
	Archive << SkillPoints;

	OutData = static_cast<TArray<unsigned char>>(Archive);
}

void ULevelingComponent::LoadData_Implementation(const TArray<uint8>& InData)
{
	FMemoryReader Reader(InData);

	// Загружаем данные
	Reader << CurrentLevel;
	Reader << CurrentExperience;
	Reader << SkillPoints;

	// Логирование для отладки
	UE_LOG(LogTemp, Log, TEXT("LevelingComponent: Level=%d, Experience=%d, SkillPoints=%d"),
		CurrentLevel, CurrentExperience, SkillPoints);
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
	bool check = true;
	while (LevelDataTable && check)
	{
		const auto& LevelData = GetLevelData(CurrentLevel);
		if (!LevelData || CurrentExperience < LevelData->RequiredXP)
		{
			check = false;
			break;
		}

		CurrentExperience -= LevelData->RequiredXP;
		CurrentLevel++;
		SkillPoints += LevelData->SkillPointsReward;

		// Вызываем событие
		OnLevelUp.Broadcast(CurrentLevel);
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
	if (const auto& LevelData = GetLevelData(CurrentLevel))
	{
		return LevelData->RequiredXP - CurrentExperience;
	}
	return 0;
}

FLevelingSaveData ULevelingComponent::SaveData() const
{
	FLevelingSaveData Data;
	Data.CurrentLevel = CurrentLevel;
	Data.CurrentExperience = CurrentExperience;
	Data.SkillPoints = SkillPoints;
	return Data;
}

void ULevelingComponent::LoadData(const FLevelingSaveData& Data)
{
	CurrentLevel = Data.CurrentLevel;
	CurrentExperience = Data.CurrentExperience;
	SkillPoints = Data.SkillPoints;
}

void ULevelingComponent::SetLevelDataTable(UDataTable* NewLevelDataTable)
{
	LevelDataTable = NewLevelDataTable;
}

const FLevelData* ULevelingComponent::GetLevelData(int32 Level) const
{
	if (!LevelDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("LevelDataTable is null"));
		return nullptr;
	}

	// Проходим по всем строкам в таблице
	for (const TPair<FName, uint8*>& Row : LevelDataTable->GetRowMap())
	{
		const FLevelData* RowData = reinterpret_cast<FLevelData*>(Row.Value);
		if (RowData && RowData->Level == Level)
		{
			return RowData;
		}
	}

	// Если уровень не найден
	UE_LOG(LogTemp, Warning, TEXT("Level %d not found in LevelDataTable"), Level);
	return nullptr;
}
