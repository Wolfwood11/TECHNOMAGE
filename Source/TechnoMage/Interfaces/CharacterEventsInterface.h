// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TechnoMage/Modificators/ModifierData.h"
#include "UObject/Interface.h"
#include "CharacterEventsInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UCharacterEventsInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class TECHNOMAGE_API ICharacterEventsInterface
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Setters")
	void OnLevelUp(int32 NewLevel);
};
