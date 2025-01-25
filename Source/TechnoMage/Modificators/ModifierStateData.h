#pragma once

#include "CoreMinimal.h"
#include "ModifierData.h"
#include "Particles/ParticleSystemComponent.h"
#include "NiagaraComponent.h"
#include "ModifierStateData.generated.h"

// Структура для хранения состояния модификатора
USTRUCT(BlueprintType)
struct FModifierStateData
{
	GENERATED_BODY()

	// Модификатор
	UPROPERTY()
	TObjectPtr<UModifierData> Modifier;

	// Время действия модификатора
	UPROPERTY()
	float ActiveTime;

	// Ссылка на Cascade эффект
	UPROPERTY()
	TObjectPtr<UParticleSystemComponent> CascadeEffect;

	// Ссылка на Niagara эффект
	UPROPERTY()
	TObjectPtr<UNiagaraComponent> NiagaraEffect;

	FModifierStateData()
		: Modifier(nullptr),
		ActiveTime(0.f),
		CascadeEffect(nullptr),
		NiagaraEffect(nullptr)
	{
	}
};
