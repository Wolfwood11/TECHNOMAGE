#include "QuestFactory.h"
#include "AssetTypeCategories.h"

UQuestFactory::UQuestFactory()
{
	bCreateNew = true;          // Разрешаем создавать новые объекты
	bEditAfterNew = true;       // Автоматически открываем объект после создания
	SupportedClass = UQuest::StaticClass();
}

UObject* UQuestFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<UQuest>(InParent, Class, Name, Flags);
}

bool UQuestFactory::ShouldShowInNewMenu() const
{
	return true;
}

FText UQuestFactory::GetDisplayName() const
{
	return FText::FromString(TEXT("Quest Data"));
}

uint32 UQuestFactory::GetMenuCategories() const
{
	return EAssetTypeCategories::Misc;
}

FText UQuestFactory::GetToolTip() const
{
	return FText::FromString(TEXT("A data asset used to define modifiers for abilities or effects."));
}

FString UQuestFactory::GetDefaultNewAssetName() const
{
	return FString(TEXT("NewQuestData"));
}
