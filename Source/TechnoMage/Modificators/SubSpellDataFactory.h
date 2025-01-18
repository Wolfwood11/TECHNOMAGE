#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "ModifierData.h"
#include "SubSpellDataFactory.generated.h"

/**
 * Фабрика для создания объектов UModifierData
 */
UCLASS(hidecategories = Object)
class TECHNOMAGE_API USubSpellDataFactory : public UFactory
{
	GENERATED_BODY()

public:
	USubSpellDataFactory();

	// UFactory Interface
	virtual FText GetDisplayName() const override;
	virtual uint32 GetMenuCategories() const override;
	virtual FText GetToolTip() const override;
	virtual FString GetDefaultNewAssetName() const override;
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	virtual bool ShouldShowInNewMenu() const override;
};
