#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TechnoMage/Components/StatsModifiersComponent.h"
#include "TechnoMage/Interfaces/CharacterGetersInterface.h"
#include "TechnoMage/Interfaces/DamageableInterface.h"
#include "BaseCharacter.generated.h"

class UCharacterResourcePool;

UCLASS()
class TECHNOMAGE_API ABaseCharacter : public ACharacter, public IDamageableInterface, public ICharacterGetersInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();

	// Реализация интерфейсов
	virtual void ApplyDamage(const FDamageResult& damageResult) override;
	virtual bool IsAlive() const override;
	virtual void Die();

	virtual TArray<UModifierData*> GetModifiers_Implementation(EModifierType Type) const override;
	virtual float GetHealth_Implementation() const override;
	virtual float GetMana_Implementation() const override;
	virtual float GetMaxHealth_Implementation() const override;
	virtual float GetMaxMana_Implementation() const override;
	virtual void AddExp_Implementation(int exp) override;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	void ProcessSpeedModifiers() const;
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StatModifiers", meta = (AllowPrivateAccess = "true"))
	UStatsModifiersComponent* StatsModifiersComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Resources", meta = (AllowPrivateAccess = "true"))
	UCharacterResourcePool* HealthPool;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float maxSpeed = 0;
};
