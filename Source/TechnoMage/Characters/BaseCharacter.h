#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TechnoMage/Components/StatsModifiersComponent.h"
#include "TechnoMage/Interfaces/ActionLockInterface.h"
#include "TechnoMage/Interfaces/CharacterEventsInterface.h"
#include "TechnoMage/Interfaces/CharacterGetersInterface.h"
#include "TechnoMage/Interfaces/CharacterSettersInterface.h"
#include "TechnoMage/Interfaces/DamageableInterface.h"
#include "BaseCharacter.generated.h"

class UCharacterResourcePool;

UCLASS()
class TECHNOMAGE_API ABaseCharacter : public ACharacter,
	public IDamageableInterface,
	public ICharacterGetersInterface,
	public ICharacterSettersInterface,
	public ICharacterEventsInterface,
	public IActionLockInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Реализация интерфейсов
	virtual void ApplyDamage_Implementation(const FDamageResult& damageResult) override;
	virtual bool IsAlive_Implementation() const override;
	virtual void Die();

	virtual TArray<UModifierData*> GetModifiers_Implementation(EModifierType Type) const override;
	virtual float GetHealth_Implementation() const override;
	virtual float GetMana_Implementation() const override;
	virtual float GetMaxHealth_Implementation() const override;
	virtual float GetMaxMana_Implementation() const override;
	virtual void AddExp_Implementation(int exp) override;


	virtual bool Lock_Implementation(TSubclassOf<UActorComponent> ComponentClass, bool DisableMovement) override;
	void UnlockActions();
	virtual bool UnLock_Implementation(TSubclassOf<UActorComponent> ComponentClass) override;
	virtual bool IsLocked_Implementation() const override;
	virtual bool IsLockedByMe_Implementation(TSubclassOf<UActorComponent> ComponentClass) const override;


protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	void ProcessSpeedModifiers() const;
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StatModifiers", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStatsModifiersComponent> StatsModifiersComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Resources", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCharacterResourcePool> HealthPool;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float maxSpeed = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float maxLockTime = 2.f;

	TSubclassOf<UActorComponent> CurrentLockComponent;
	float CurrentLockTime = 0.f;
	bool CurrentDisableMovement = false;
};
