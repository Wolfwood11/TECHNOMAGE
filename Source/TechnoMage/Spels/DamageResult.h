#pragma once

#include "CoreMinimal.h"
#include "SpellElement.h"
#include "TechnoMage/Modificators/ModifierData.h"
#include "DamageResult.generated.h"

USTRUCT(BlueprintType)
struct FDamageResult
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	bool bIsCritical;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	ESpellElement Element;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	TArray<UModifierData*> Modifiers;

	FDamageResult()
		: Damage(0.0f), bIsCritical(false), Element(ESpellElement::Normal) {}
};
