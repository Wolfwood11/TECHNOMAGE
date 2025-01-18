#include "SubSpellDataFactory.h"
#include "AssetTypeCategories.h"
#include "SubSpell.h"

USubSpellDataFactory::USubSpellDataFactory()
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = USubSpell::StaticClass();
}

UObject* USubSpellDataFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<USubSpell>(InParent, Class, Name, Flags);
}

bool USubSpellDataFactory::ShouldShowInNewMenu() const
{
	return true;
}

FText USubSpellDataFactory::GetDisplayName() const
{
	return FText::FromString(TEXT("SubSpell Data"));
}

uint32 USubSpellDataFactory::GetMenuCategories() const
{
	return EAssetTypeCategories::Misc;
}

FText USubSpellDataFactory::GetToolTip() const
{
	return FText::FromString(TEXT("A data asset used to define SubSpell for abilities or effects."));
}

FString USubSpellDataFactory::GetDefaultNewAssetName() const
{
	return FString(TEXT("NewSubSpellData"));
}
