// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TechnoMage/Modificators/ModifierData.h"
#include "UObject/Interface.h"
#include "CharacterSettersInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UCharacterSettersInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class TECHNOMAGE_API ICharacterSettersInterface
{
	GENERATED_BODY()
public:
	// Распределение стата
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Setters")
	void AllocateStat(ECharacterStatType StatType);

	// Добавление опыта
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Setters")
	void AddExp(int32 Exp);
};
