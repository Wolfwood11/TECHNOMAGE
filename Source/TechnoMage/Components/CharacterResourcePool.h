#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TechnoMage/Enums/Enums.h"
#include "TechnoMage/Interfaces/SaveableInterface.h"
#include "CharacterResourcePool.generated.h"

// Делегат для уведомления владельца
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnResourceDepleted);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TECHNOMAGE_API UCharacterResourcePool : public UActorComponent, public ISaveableInterface
{
	GENERATED_BODY()

public:
	UCharacterResourcePool();

	virtual void SaveData_Implementation(TArray<uint8>& OutData) override;
	virtual void LoadData_Implementation(const TArray<uint8>& InData) override;

	// Установить новое значение ресурса
	UFUNCTION(BlueprintCallable, Category = "Resource Pool")
	void SetCurrentResource(float NewValue);

	UFUNCTION(BlueprintCallable, Category = "Resource Pool")
	void SetAllowNegativeResource(bool newVal);

	UFUNCTION(BlueprintCallable, Category = "Resource Pool")
	void SetMaxResource(float newVal);

	UFUNCTION(BlueprintCallable, Category = "Resource Pool")
	void SetRegenRate(float newVal);

	UFUNCTION(BlueprintCallable, Category = "Resource Pool")
	float GetMaxResource() const;

	UFUNCTION(BlueprintCallable, Category = "Resource Pool")
	float GetRegenRate() const;

	UFUNCTION(BlueprintCallable, Category = "Resource Pool")
	bool GetAllowNegativeResource() const;

	// Геттер для текущего значения ресурса
	UFUNCTION(BlueprintCallable, Category = "Resource Pool")
	float GetCurrentResource() const;

	// Проверяет, достаточно ли ресурса для выполнения действия
	UFUNCTION(BlueprintCallable, Category = "Resource Pool")
	bool HasEnoughResource(float Amount) const;

	// Потребляет указанное количество ресурса, возвращает true, если успешно
	UFUNCTION(BlueprintCallable, Category = "Resource Pool")
	bool ConsumeResource(float Amount);

	// Потребляет указанное количество ресурса, возвращает true, если успешно
	UFUNCTION(BlueprintCallable, Category = "Resource Pool")
	void AddResource(float Amount);

	// Потребляет указанное количество ресурса, возвращает true, если успешно
	UFUNCTION(BlueprintCallable, Category = "Resource Pool")
	void SetModifierTypeAffects(EModifierType type);

protected:
	virtual void BeginPlay() override;

	virtual void ProcessModifiers(float DeltaTime);

	// Восстановление ресурса
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Pool")
	EModifierType ModifierTypeAffects;

	// Текущее значение ресурса
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Resource Pool")
	float CurrentResource;

	// Максимальное значение ресурса
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Pool", meta = (ClampMin = "0.0"))
	float MaxResource;

	// Скорость восстановления ресурса (в единицах в секунду)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Pool", meta = (ClampMin = "0.0"))
	float RegenRate;

	// Разрешить потребление ниже 0
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Pool")
	bool bAllowNegativeResource;

	// Делегат для уведомления, когда ресурс достигает нуля или ниже
	UPROPERTY(BlueprintAssignable, Category = "Resource Pool")
	FOnResourceDepleted OnResourceDepleted;

private:
	// Обновление текущего значения ресурса
	void RegenerateResource(float DeltaTime);
};
