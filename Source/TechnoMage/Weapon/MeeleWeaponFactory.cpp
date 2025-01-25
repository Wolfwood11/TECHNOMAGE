#include "MeeleWeaponFactory.h"
#include "MeeleWeapon.h"
#include "AssetTypeCategories.h"

UMeeleWeaponFactory::UMeeleWeaponFactory()
{
	bCreateNew = true;          // Разрешаем создавать новые объекты
	bEditAfterNew = true;       // Автоматически открываем объект после создания
	SupportedClass = UMeeleWeapon::StaticClass();
}

UObject* UMeeleWeaponFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<UMeeleWeapon>(InParent, Class, Name, Flags);
}

bool UMeeleWeaponFactory::ShouldShowInNewMenu() const
{
	return true;
}

FText UMeeleWeaponFactory::GetDisplayName() const
{
	return FText::FromString(TEXT("MeeleWeapon Data"));
}

uint32 UMeeleWeaponFactory::GetMenuCategories() const
{
	return EAssetTypeCategories::Misc;
}

FText UMeeleWeaponFactory::GetToolTip() const
{
	return FText::FromString(TEXT("A data asset used to define MeeleWeapon Data."));
}

FString UMeeleWeaponFactory::GetDefaultNewAssetName() const
{
	return FString(TEXT("NewMeeleWeapon"));
}
