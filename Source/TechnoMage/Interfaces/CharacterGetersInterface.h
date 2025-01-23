// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TechnoMage/Modificators/ModifierData.h"
#include "UObject/Interface.h"
#include "CharacterGetersInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UCharacterGetersInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class TECHNOMAGE_API ICharacterGetersInterface
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Getters")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Getters")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Getters")
	float GetMana() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Getters")
	float GetMaxMana() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Getters")
	int32 GetLevel() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Getters")
	int32 GetExp() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Getters")
	int32 GetExpToNextLevel() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Modifiers")
	TArray<UModifierData*> GetModifiers(EModifierType Type) const;

	// Получение множителя стата
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Getters")
	float GetStatMultiplier(ECharacterStatType StatType) const;

	// Получение текущего значения стата
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Getters")
	int32 GetStatValue(ECharacterStatType StatType) const;

	// Получение стоимости повышения стата
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Getters")
	int32 GetStatUpgradeCost(ECharacterStatType StatType) const;

	// Получение доступных очков
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Getters")
	int32 GetAvailableStatPoints() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Character Getters")
	float GetCharacterParam(ECharacterParamType ParamType) const;
};
