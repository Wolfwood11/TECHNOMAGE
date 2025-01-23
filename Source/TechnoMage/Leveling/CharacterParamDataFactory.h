#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "CharacterParamDataFactory.generated.h"

/**
 * Фабрика для создания объектов UQuest
 */
UCLASS()
class TECHNOMAGE_API UCharacterParamDataFactory : public UFactory
{
	GENERATED_BODY()

public:
	UCharacterParamDataFactory();

	// UFactory Interface
	virtual FText GetDisplayName() const override;
	virtual uint32 GetMenuCategories() const override;
	virtual FText GetToolTip() const override;
	virtual FString GetDefaultNewAssetName() const override;
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	virtual bool ShouldShowInNewMenu() const override;
};