#include "StatsModifiersComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "TechnoMage/Spels/DamageResult.h"
#include "NiagaraComponent.h"
#include "GameFramework/Actor.h"
#include "TechnoMage/ObjectPool.h"
#include "TechnoMage/Modificators/SubSpell.h"
#include "TechnoMage/Spels/BaseSpell.h"
#include "TechnoMage/Subsystems/ActorTrackingSubsystem.h"

UStatsModifiersComponent::UStatsModifiersComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	TimeSinceLastSave = 0.0f;
	SaveInterval = 5.0f; // Интервал сохранения в секундах
}

bool UStatsModifiersComponent::AreElementsOpposite(ESpellElement Element1, ESpellElement Element2)
{
	switch (Element1)
	{
	case ESpellElement::Fire:
		return Element2 == ESpellElement::Water;
	case ESpellElement::Water:
		return Element2 == ESpellElement::Fire;
	case ESpellElement::Earth:
		return Element2 == ESpellElement::Air;
	case ESpellElement::Air:
		return Element2 == ESpellElement::Earth;
	default:
		return false; // Normal и Chaos не имеют противоположностей
	}
}

bool UStatsModifiersComponent::AreElementsBoosts(ESpellElement Element1, ESpellElement Element2)
{
	switch (Element1)
	{
	case ESpellElement::Fire:
		return Element2 == ESpellElement::Earth;
	case ESpellElement::Water:
		return Element2 == ESpellElement::Air;
	case ESpellElement::Earth:
		return Element2 == ESpellElement::Fire;
	case ESpellElement::Air:
		return Element2 == ESpellElement::Water;
	default:
		return false;
	}
}

TArray<UModifierData*> UStatsModifiersComponent::GetModifiers(EModifierType Type) const
{
	TArray<UModifierData*> FilteredModifiers;

	for (const FModifierStateData& ModifierState : ActiveModifiers)
	{
		if (ModifierState.Modifier && ModifierState.Modifier->ModifierType == Type)
		{
			FilteredModifiers.Add(ModifierState.Modifier);
		}
	}

	return FilteredModifiers;
}

void UStatsModifiersComponent::RemoveOpposite(ESpellElement element)
{
	for (int32 i = ActiveModifiers.Num() - 1; i >= 0; --i)
	{
		FModifierStateData& ModifierState = ActiveModifiers[i];
		if (AreElementsOpposite(element, ModifierState.Modifier->ModifierElement))
			ActiveModifiers.RemoveAt(i);
	}
}

void UStatsModifiersComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UStatsModifiersComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for (int32 i = ActiveModifiers.Num() - 1; i >= 0; --i)
	{
		FModifierStateData& ModifierState = ActiveModifiers[i];
		ModifierState.ActiveTime -= DeltaTime;

		if (ModifierState.ActiveTime <= 0.0f)
		{
			DeactivateEffects(ModifierState);
			ActiveModifiers.RemoveAt(i);
		}
	}

	TimeSinceLastSave += DeltaTime;
	if (TimeSinceLastSave >= SaveInterval)
	{
		SaveModifiers();
		TimeSinceLastSave = 0.0f;
	}
}

void UStatsModifiersComponent::ApplyModifier(const FDamageResult& DamageResult)
{
	for (FModifierStateData& ModifierState : ActiveModifiers)
	{
		if (ModifierState.Modifier && IsElementalModifier(ModifierState.Modifier->ModifierElement))
		{
			HandleSubSpells(DamageResult, ModifierState);
			return;
		}
	}

	// Если нет активного элементального модификатора, проверяем шанс наложения
	for (UModifierData* Modifier : DamageResult.Modifiers)
	{
		if (!Modifier || !IsElementalModifier(Modifier->ModifierElement))
		{
			continue;
		}

		float RandomChance = FMath::FRandRange(0.0f, 100.0f);
		if (RandomChance <= Modifier->Chance)
		{
			// Накладываем новый элементальный модификатор
			FModifierStateData NewModifierState;
			NewModifierState.Modifier = Modifier;
			NewModifierState.ActiveTime = Modifier->Duration;
			ActivateEffects(NewModifierState);
			ActiveModifiers.Add(NewModifierState);

			UE_LOG(LogTemp, Log, TEXT("Elemental Modifier %s applied with chance %.2f"), *Modifier->GetName(), Modifier->Chance);
			return; // Завершаем, так как может быть только один активный элементальный модификатор
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Elemental Modifier %s failed with chance %.2f"), *Modifier->GetName(), Modifier->Chance);
		}
	}
}

void UStatsModifiersComponent::RemoveModifier(const UModifierData* Modifier)
{
	for (int32 i = ActiveModifiers.Num() - 1; i >= 0; --i)
	{
		if (ActiveModifiers[i].Modifier == Modifier)
		{
			ActiveModifiers.RemoveAt(i);
			break;
		}
	}
}

void UStatsModifiersComponent::HandleSubSpells(const FDamageResult& DamageResult, FModifierStateData& ActiveElementalModifier)
{
	const auto& modifier = ActiveElementalModifier.Modifier;
	if (!modifier)
	{
		return;
	}

	for (const USubSpell* SubSpellData : modifier->SubSpells)
	{
		if (!SubSpellData)
		{
			continue;
		}

		if (SubSpellData->InputElement == DamageResult.Element)
		{
			// Шанс отмены текущего эффекта
			float DiscardRandomChance = FMath::FRandRange(0.0f, 100.0f);
			if (DiscardRandomChance <= SubSpellData->DiscardProbability)
			{
				DeactivateEffects(ActiveElementalModifier);
				RemoveModifier(modifier);
				return;
			}

			// Шанс создания субзаклинания
			float SpawnRandomChance = FMath::FRandRange(0.0f, 100.0f);
			if (SpawnRandomChance <= SubSpellData->Probability && SubSpellData->SubSpell)
			{
				SpawnSubSpell(SubSpellData->SubSpell);
				UE_LOG(LogTemp, Log, TEXT("SubSpell spawned: %s"), *SubSpellData->SubSpell->GetName());
			}
		}
	}
}

bool UStatsModifiersComponent::IsElementalModifier(ESpellElement Element)
{
	switch (Element)
	{
	case ESpellElement::Fire:
	case ESpellElement::Water:
	case ESpellElement::Earth:
	case ESpellElement::Air:
		return true;

	case ESpellElement::Normal:
	case ESpellElement::Chaos:
	default:
		return false;
	}
}

void UStatsModifiersComponent::SpawnSubSpell(const TSubclassOf<ABaseSpell>& SubSpell) const
{
	if (!SubSpell)
	{
		return;
	}

	AObjectPool* objectPool = nullptr;
	if (UActorTrackingSubsystem* Subsystem = GetWorld()->GetSubsystem<UActorTrackingSubsystem>())
	{
		objectPool = Subsystem->GetRegisteredActor();
	}

	if (objectPool)
	{
		if (ABaseSpell* Spell = Cast<ABaseSpell>(objectPool->GetObject(SubSpell)))
		{
			Spell->ActivateFromPoll(GetOwner()->GetActorTransform(), GetOwner());
		}
	}
}

void UStatsModifiersComponent::DeactivateEffects(FModifierStateData& ModifierState)
{
	UE_LOG(LogTemp, Log, TEXT("DeactivateEffects called for Modifier %s"), ModifierState.Modifier ? *ModifierState.Modifier->GetName() : TEXT("Unknown"));
	if (ModifierState.CascadeEffect)
	{
		ModifierState.CascadeEffect->Deactivate();
		ModifierState.CascadeEffect->DestroyComponent();
		ModifierState.CascadeEffect = nullptr;
	}

	if (ModifierState.NiagaraEffect)
	{
		ModifierState.NiagaraEffect->Deactivate();
		ModifierState.NiagaraEffect->DestroyComponent();
		ModifierState.NiagaraEffect = nullptr;
	}
}

void UStatsModifiersComponent::ActivateEffects(FModifierStateData& ModifierState)
{
	if (!ModifierState.Modifier)
	{
		return;
	}

	AActor* OwnerActor = GetOwner();
	if (!OwnerActor)
	{
		return;
	}

	// Активируем каскадный эффект
	if (ModifierState.Modifier->ApplyCascadeEffect)
	{
		UParticleSystemComponent* NewCascadeEffect = NewObject<UParticleSystemComponent>(OwnerActor);
		if (NewCascadeEffect)
		{
			NewCascadeEffect->SetTemplate(ModifierState.Modifier->ApplyCascadeEffect);
			NewCascadeEffect->SetupAttachment(OwnerActor->GetRootComponent());
			NewCascadeEffect->RegisterComponent();
			NewCascadeEffect->Activate(true);

			ModifierState.CascadeEffect = NewCascadeEffect;
		}
	}

	// Активируем ниагарский эффект
	if (ModifierState.Modifier->ApplyNiagaraEffect)
	{
		UNiagaraComponent* NewNiagaraEffect = NewObject<UNiagaraComponent>(OwnerActor);
		if (NewNiagaraEffect)
		{
			NewNiagaraEffect->SetAsset(ModifierState.Modifier->ApplyNiagaraEffect);
			NewNiagaraEffect->SetupAttachment(OwnerActor->GetRootComponent());
			NewNiagaraEffect->RegisterComponent();
			NewNiagaraEffect->Activate(true);

			ModifierState.NiagaraEffect = NewNiagaraEffect;
		}
	}
}


void UStatsModifiersComponent::ClearAllModifiers()
{
	for (FModifierStateData& ModifierState : ActiveModifiers)
	{
		DeactivateEffects(ModifierState);
	}
	ActiveModifiers.Empty();
}

void UStatsModifiersComponent::ClearModifiersOfType(EModifierType Type)
{
	for (int32 i = ActiveModifiers.Num() - 1; i >= 0; --i)
	{
		if (ActiveModifiers[i].Modifier->ModifierType == Type)
		{
			DeactivateEffects(ActiveModifiers[i]);
			ActiveModifiers.RemoveAt(i);
		}
	}
}

void UStatsModifiersComponent::SaveModifiers()
{
	UE_LOG(LogTemp, Log, TEXT("Saving modifiers..."));

	// Реализуйте сохранение состояния модификаторов (например, через USaveGame)
	// Сохраните ModifierType, ActiveTime и другие данные для восстановления после перезапуска.
}

void UStatsModifiersComponent::LoadModifiers()
{
	UE_LOG(LogTemp, Log, TEXT("Loading modifiers..."));

	// Реализуйте загрузку модификаторов из сохранения
	// После загрузки заново активируйте эффекты через ApplyModifier
}
