// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "Enums.generated.h"

/**
 *
 */
UCLASS()
class TECHNOMAGE_API UEnums : public UUserDefinedEnum
{
	GENERATED_BODY()

};

UENUM(BlueprintType)
enum class EModifierType : uint8
{
	Health      UMETA(DisplayName = "Health"),        // Модификатор здоровья
	Speed       UMETA(DisplayName = "Speed"),         // Модификатор скорости
	Armor       UMETA(DisplayName = "Armor"),         // Модификатор брони
	Attack      UMETA(DisplayName = "Attack"),        // Модификатор атаки
	MagicPower  UMETA(DisplayName = "Magic Power"),   // Модификатор магической силы
	Resistance  UMETA(DisplayName = "Resistance"),    // Модификатор сопротивления
	Stamina     UMETA(DisplayName = "Stamina"),       // Модификатор выносливости
	Mana        UMETA(DisplayName = "Mana"),          // Модификатор маны
	CritChance  UMETA(DisplayName = "Crit Chance"),   // Модификатор шанса критического удара
	CritDamage  UMETA(DisplayName = "Crit Damage"),    // Модификатор критического урона
	PermamentDeath  UMETA(DisplayName = "Permament Death")    // Модификатор мгновенная смерть
};

UENUM(BlueprintType)
enum class ECharacterStatType : uint8
{
	Strength UMETA(DisplayName = "Strength"),
	Vitality UMETA(DisplayName = "Vitality"),
	Agility UMETA(DisplayName = "Agility"),
	Dexterity UMETA(DisplayName = "Dexterity"),
	Intelligence UMETA(DisplayName = "Intelligence"),
};

UENUM(BlueprintType)
enum class ECharacterParamType : uint8
{
	AtkMultiplier UMETA(DisplayName = "AtkMultiplier"),
	MatkMultiplier UMETA(DisplayName = "MatkMultiplier"),
	ArmorMultiplier  UMETA(DisplayName = "ArmorMultiplier"),
	CastDelayMultiplier UMETA(DisplayName = "CastDelayMultiplier"),
	AttackSpeed UMETA(DisplayName = "AttackSpeed"),
	Evasion UMETA(DisplayName = "Evasion"),
	Velocity UMETA(DisplayName = "Velocity"),
	MaxHpMultiplier UMETA(DisplayName = "MaxHpMultiplier"),
	MaxSpMultiplier UMETA(DisplayName = "MaxSpMultiplier"),
	HpRegenMultiplier UMETA(DisplayName = "HpRegenMultiplier"),
	SpRegenMultiplier UMETA(DisplayName = "SpRegenMultiplier"),
};

UENUM(BlueprintType)
enum class EModifierOperationType : uint8
{
	Add         UMETA(DisplayName = "Add"),           // Добавление (например, +10)
	Multiply    UMETA(DisplayName = "Multiply"),      // Умножение (например, *1.5
	Replace     UMETA(DisplayName = "Replace"),       // Замена текущего значения
	Subtract    UMETA(DisplayName = "Subtract"),      // Вычитание (например, -10)
	Divide      UMETA(DisplayName = "Divide"),        // Деление (например, /2)
	Min         UMETA(DisplayName = "Min"),           // Установить минимальное значение
	Max         UMETA(DisplayName = "Max")            // Установить максимальное значение
};
