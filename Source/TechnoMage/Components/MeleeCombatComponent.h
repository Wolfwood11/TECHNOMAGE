#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TechnoMage/Spels/DamageResult.h"
#include "MeleeCombatComponent.generated.h"

class UCapsuleComponent;
class UMeeleWeapon;

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	Normal UMETA(DisplayName = "Normal Attack"),
	Strong UMETA(DisplayName = "Strong Attack")
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TECHNOMAGE_API UMeleeCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMeleeCombatComponent();

	// Устанавливает текущее оружие
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void EquipWeapon(UMeeleWeapon* NewWeapon);

	// Выполняет атаку
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void PerformAttack(EAttackType AttackType);

protected:
	virtual void BeginPlay() override;

private:
	// Текущее оружие
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UMeeleWeapon* CurrentWeapon;

	// Статический меш оружия
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* WeaponMesh;

	// Компонент коллизии для меча
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* WeaponCollision;

	// Сокет для привязки оружия
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	FName WeaponSocketName = TEXT("WeaponSocket");

	// Привязка оружия к сокету
	void AttachWeaponToSocket();

	// Адаптация капсулы под размеры меча
	void AdjustCollisionSize();

	UFUNCTION()
	void HandleAnimationNotify(FName NotifyName, const FBranchingPointNotifyPayload& Payload);

	// Обработчик нанесения урона
	UFUNCTION()
	void OnWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Активация/деактивация коллизии
	void EnableWeaponCollision();
	void DisableWeaponCollision();

	// Подсчёт урона
	FDamageResult CalculateDamage(float InitialDamage, int TargetIndex, EAttackType AttackType) const;

	// Проверка кулдауна
	bool IsAttackOnCooldown() const;
	void StartCooldown(float CooldownTime);

	// Таймер кулдауна
	FTimerHandle AttackCooldownTimer;

	// Состояние кулдауна
	bool bIsOnCooldown = false;

	// Обработка цели, чтобы наносить урон один раз
	TArray<AActor*> ProcessedTargets;
};
