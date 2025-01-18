#pragma once

#include "CoreMinimal.h"
#include "BaseSpell.h"
#include "AreaSpell.generated.h"

UCLASS()
class TECHNOMAGE_API AAreaSpell : public ABaseSpell
{
	GENERATED_BODY()

public:
	AAreaSpell();

protected:
	virtual void BeginPlay() override;

	// Обработка эффекта активации
	virtual void PlayActivationEffect() override;

	// Обработка эффекта столкновения
	virtual void TriggerEffect(const FHitResult& ImpactResult) override;

public:
	// Устанавливаем радиус удара
	UFUNCTION(BlueprintCallable, Category = "Area Spell")
	void SetAreaRadius(float NewRadius);

protected:
	// Радиус удара
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration", meta = (AllowPrivateAccess = "true"))
	float AreaRadius;

	// Время активации перед возвратом в пул
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Configuration", meta = (AllowPrivateAccess = "true"))
	float ActivationTime;
	UPROPERTY()
	TSet<AActor*> ProcessedActors;

	virtual void ReturnToPool() override;

private:
	// Таймер для возврата заклинания в пул
	FTimerHandle ActivationTimer;

	// Наносит урон всем, кто находится в радиусе
	void DealDamage();
};
