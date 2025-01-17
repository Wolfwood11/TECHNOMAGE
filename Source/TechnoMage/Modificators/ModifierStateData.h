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
	UModifierData* Modifier;

	// Время действия модификатора
	UPROPERTY()
	float ActiveTime;

	// Ссылка на Cascade эффект
	UPROPERTY()
	UParticleSystemComponent* CascadeEffect;

	// Ссылка на Niagara эффект
	UPROPERTY()
	UNiagaraComponent* NiagaraEffect;

	FModifierStateData()
		: Modifier(nullptr),
		ActiveTime(0.f),
		CascadeEffect(nullptr),
		NiagaraEffect(nullptr)
	{
	}
};
