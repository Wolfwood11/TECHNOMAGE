#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DashComponent.generated.h"

class UCharacterResourcePool;
class UNiagaraSystem;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TECHNOMAGE_API UDashComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDashComponent();
	void SetManaPool(UCharacterResourcePool* manaPoll);
	void SetDashEffect(UParticleSystemComponent* effect);

	// Выполнить рывок
	UFUNCTION(BlueprintCallable, Category = "Dash")
	void PerformDash();

	UFUNCTION(BlueprintCallable, Category = "Dash")
	bool DashInPrecess() const;

protected:
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	// Проверка, можно ли выполнить рывок
	bool CanDash() const;

	// Запустить эффект рывка
	void PlayDashEffect();

	// Начать рывок
	void StartDash();

	// Завершить рывок
	void EndDash();

	// Расстояние рывка
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dash", meta = (AllowPrivateAccess = "true"))
	float DashDistance;

	// Время перезарядки
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dash", meta = (AllowPrivateAccess = "true"))
	float Cooldown;

	// Стоимость маны для рывка
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dash", meta = (AllowPrivateAccess = "true"))
	float ManaCost;

	// Время последнего рывка
	float LastDashTime;

	// Время, за которое выполняется рывок
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dash", meta = (AllowPrivateAccess = "true"))
	float DashDuration;

	// Направление рывка
	FVector DashDirection;

	// Начальная позиция рывка
	FVector StartLocation;

	// Конечная позиция рывка
	FVector EndLocation;

	// Таймер рывка
	float DashElapsedTime;

	// Активен ли рывок
	bool bIsDashing;

	// Ссылка на компонент маны
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash", meta = (AllowPrivateAccess = "true"))
	UCharacterResourcePool* ManaPool;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effects", meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* DashEffectComponent;
};
