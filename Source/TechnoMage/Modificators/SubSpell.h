// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Engine/Texture2D.h"         
#include "TechnoMage/Spels/BaseSpell.h"
#include "TechnoMage/Spels/SpellElement.h"
#include "SubSpell.generated.h"

UCLASS(Blueprintable, BlueprintType)
class TECHNOMAGE_API  USubSpell : public UDataAsset
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spell")
	ESpellElement InputElement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spell")
	float Probability = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spell")
	TSubclassOf<ABaseSpell> SubSpell;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spell")
	float DiscardProbability = 10.f;

	USubSpell()
		: InputElement(ESpellElement::Normal), SubSpell(nullptr) {}
};

