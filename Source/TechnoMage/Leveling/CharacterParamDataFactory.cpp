#include "CharacterParamDataFactory.h"

#include "AssetTypeCategories.h"
#include "LevelData.h"

UCharacterParamDataFactory::UCharacterParamDataFactory()
{
	bCreateNew = true;          // Разрешаем создавать новые объекты
	bEditAfterNew = true;       // Автоматически открываем объект после создания
	SupportedClass = UCharacterParamData::StaticClass();
}

UObject* UCharacterParamDataFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<UCharacterParamData>(InParent, Class, Name, Flags);
}

bool UCharacterParamDataFactory::ShouldShowInNewMenu() const
{
	return true;
}

FText UCharacterParamDataFactory::GetDisplayName() const
{
	return FText::FromString(TEXT("CharacterParam Data"));
}

uint32 UCharacterParamDataFactory::GetMenuCategories() const
{
	return EAssetTypeCategories::Misc;
}

FText UCharacterParamDataFactory::GetToolTip() const
{
	return FText::FromString(TEXT("A data asset used to define CharacterParam Data."));
}

FString UCharacterParamDataFactory::GetDefaultNewAssetName() const
{
	return FString(TEXT("NewCharacterParamData"));
}
