#include "StatsDataFactory.h"
#include "AssetTypeCategories.h"
#include "LevelData.h"

UStatsDataFactory::UStatsDataFactory()
{
	bCreateNew = true;          // Разрешаем создавать новые объекты
	bEditAfterNew = true;       // Автоматически открываем объект после создания
	SupportedClass = UStatsData::StaticClass();
}

UObject* UStatsDataFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<UStatsData>(InParent, Class, Name, Flags);
}

bool UStatsDataFactory::ShouldShowInNewMenu() const
{
	return true;
}

FText UStatsDataFactory::GetDisplayName() const
{
	return FText::FromString(TEXT("Stats Data"));
}

uint32 UStatsDataFactory::GetMenuCategories() const
{
	return EAssetTypeCategories::Misc;
}

FText UStatsDataFactory::GetToolTip() const
{
	return FText::FromString(TEXT("A data asset used to define Stats Data."));
}

FString UStatsDataFactory::GetDefaultNewAssetName() const
{
	return FString(TEXT("NewStatsData"));
}
