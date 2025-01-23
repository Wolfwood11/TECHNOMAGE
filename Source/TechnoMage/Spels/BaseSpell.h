#pragma once

#include "CoreMinimal.h"
#include "DamageResult.h"
#include "SpellElement.h"
#include "..//BasePoolableActor.h"
#include "Components/CapsuleComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "TechnoMage/Modificators/ModifierData.h"
#include "BaseSpell.generated.h"

UCLASS()
class ABaseSpell : public ABasePoolableActor
{
	GENERATED_BODY()

public:
	ABaseSpell();

	virtual void ReturnToPool() override;

	virtual void ActivateFromPoll(const FTransform& transform, AActor* NewInstigatorActor) override;

	virtual void ActivateSpell(const FTransform& transform, AActor* NewInstigatorActor, float damageModificator = 1.f);

	UFUNCTION(BlueprintCallable, Category = "Modifiers")
	virtual void AddModifier(UModifierData* modifier);

	UFUNCTION(BlueprintCallable, Category = "Modifiers")
	virtual void RemoveModifier(UModifierData* modifier);

	UFUNCTION(BlueprintCallable, Category = "Geters")
	int GetSpellCost() const;

	UFUNCTION(BlueprintCallable, Category = "Geters")
	float GetSpellCooldown() const;

	UFUNCTION(BlueprintCallable, Category = "Geters")
	float GetDamage() const;

	UFUNCTION(BlueprintCallable, Category = "Geters")
	float GetDamageModificator() const;

	UFUNCTION(BlueprintCallable, Category = "Geters")
	float GetMaxDamage() const;

	UFUNCTION(BlueprintCallable, Category = "Geters")
	float GetCriticalChance() const;

	UFUNCTION(BlueprintCallable, Category = "Geters")
	ESpellElement GetSpellElement() const;

	UFUNCTION(BlueprintCallable, Category = "Geters")
	TArray<UModifierData*> GetModifiers() const;

	UFUNCTION(BlueprintCallable, Category = "Geters")
	FDamageResult CalculateDamage() const;

protected:
	virtual void BeginPlay() override;

	virtual void UpdateComponentsState(bool value) override;

	// Виртуальная функция для обработки эффектов столкновения
	virtual void TriggerEffect(const FHitResult& ImpactResult);

	// Обработчик события оверлапа
	UFUNCTION()
	virtual void OnCapsuleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Добавление эффекта активации
	virtual void PlayActivationEffect();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* ParticleSystemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration", meta = (AllowPrivateAccess = "true"))
	float Damage = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration", meta = (AllowPrivateAccess = "true"))
	float MaxDamage = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration", meta = (AllowPrivateAccess = "true"))
	int SpellCost = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration", meta = (AllowPrivateAccess = "true"))
	float CriticalChance = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration", meta = (AllowPrivateAccess = "true"))
	float SpellCooldown = 0.15;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration", meta = (AllowPrivateAccess = "true"))
	ESpellElement Element;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration", meta = (AllowPrivateAccess = "true"))
	TArray<UModifierData*> modifiers;

	// Эффект активации
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* ActivationEffect;

	// Эффект столкновения
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* CollisionEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RuntimeParams", meta = (AllowPrivateAccess = "true"))
	float DamageModificator = 1.f;
};
