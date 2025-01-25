// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "TechnoMage/Enums/Enums.h"
#include "Engine/Texture2D.h"         
#include "NiagaraSystem.h"            
#include "Particles/ParticleSystem.h" 
#include "TechnoMage/Spels/SpellElement.h"
#include "ModifierData.generated.h"


class USubSpell;
/**
 *
 */
UCLASS(Blueprintable, BlueprintType)
class TECHNOMAGE_API UModifierData : public UDataAsset
{
	GENERATED_BODY()

public:
	// Тип модификатора
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifier")
	EModifierType ModifierType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifier")
	ESpellElement ModifierElement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifier")
	TArray<TObjectPtr<USubSpell>> SubSpells;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifier")
	float Chance;

	// Тип действия модификатора
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifier")
	EModifierOperationType OperationType;

	// Значение модификатора
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifier")
	float ModifierValue;

	// Время действия модификатора (в секундах)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modifier")
	float Duration;

	// Иконка для отображения модификатора (в UI)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
	TObjectPtr<UTexture2D> Icon;

	// Niagara эффект при наложении
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals|Niagara")
	TObjectPtr<UNiagaraSystem> ApplyNiagaraEffect;

	// Cascade эффект при наложении
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals|Cascade")
	TObjectPtr<UParticleSystem> ApplyCascadeEffect;

	// Niagara эффект при активном состоянии
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals|Niagara")
	TObjectPtr<UNiagaraSystem> ActiveNiagaraEffect;

	// Cascade эффект при активном состоянии
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals|Cascade")
	TObjectPtr<UParticleSystem> ActiveCascadeEffect;

	// Конструктор по умолчанию
	UModifierData()
		: ModifierType(EModifierType::Health),
		ModifierElement(ESpellElement::Normal),
		Chance(10.f),
		OperationType(EModifierOperationType::Add),
		ModifierValue(0.f),
		Duration(0.f),
		Icon(nullptr),
		ApplyNiagaraEffect(nullptr),
		ApplyCascadeEffect(nullptr),
		ActiveNiagaraEffect(nullptr),
		ActiveCascadeEffect(nullptr)
	{
	}
};
