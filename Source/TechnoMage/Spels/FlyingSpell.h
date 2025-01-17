#pragma once

#include "CoreMinimal.h"
#include "BaseSpell.h"
#include "FlyingSpell.generated.h"

class UProjectileMovementComponent;

UCLASS()
class TECHNOMAGE_API AFlyingSpell : public ABaseSpell
{
	GENERATED_BODY()

public:
	AFlyingSpell();

protected:
	virtual void BeginPlay() override;

	virtual void UpdateComponentsState(bool value) override;

	virtual void TriggerEffect(const FHitResult& ImpactResult) override;

public:
	virtual void ActivateFromPoll(const FTransform& transform, AActor* NewInstigatorActor) override;
	virtual void ReturnToPool() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;
};
