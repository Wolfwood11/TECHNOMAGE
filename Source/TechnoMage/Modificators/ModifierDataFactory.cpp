#include "ModifierDataFactory.h"
#include "AssetTypeCategories.h"
#include "ModifierData.h"

UModifierDataFactory::UModifierDataFactory()
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UModifierData::StaticClass();
}

UObject* UModifierDataFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<UModifierData>(InParent, Class, Name, Flags);
}

bool UModifierDataFactory::ShouldShowInNewMenu() const
{
	return true;
}

FText UModifierDataFactory::GetDisplayName() const
{
	return FText::FromString(TEXT("Modifier Data"));
}

uint32 UModifierDataFactory::GetMenuCategories() const
{
	return EAssetTypeCategories::Misc;
}

FText UModifierDataFactory::GetToolTip() const
{
	return FText::FromString(TEXT("A data asset used to define modifiers for abilities or effects."));
}

FString UModifierDataFactory::GetDefaultNewAssetName() const
{
	return FString(TEXT("NewModifierData"));
}
