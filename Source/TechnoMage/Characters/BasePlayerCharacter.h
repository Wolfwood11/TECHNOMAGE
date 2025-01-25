#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "InputActionValue.h"
#include "TechnoMage/Interfaces/SaveableInterface.h"
#include "BasePlayerCharacter.generated.h"

class UMeleeCombatComponent;
class ULevelingComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class USpellCaster;
class UDashComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS()
class TECHNOMAGE_API ABasePlayerCharacter : public ABaseCharacter, public ISaveableInterface
{
	GENERATED_BODY()

public:
	ABasePlayerCharacter();

	// Реализация интерфейса сохранения
	virtual void SaveData_Implementation(TArray<uint8>& OutData) override;
	virtual void LoadData_Implementation(const TArray<uint8>& InData) override;

	virtual void AddExp_Implementation(int exp) override;
	virtual void AllocateStat_Implementation(ECharacterStatType StatType) override;
	virtual int32 GetLevel_Implementation() const override;
	virtual int32 GetExp_Implementation() const override;
	virtual int32 GetExpToNextLevel_Implementation() const override;
	float GetStatMultiplier_Implementation(ECharacterStatType StatType) const;

	virtual float GetMana_Implementation() const override;
	virtual float GetMaxMana_Implementation() const override;
	void SavePlayerData();
	void OnLevelUp_Implementation(int32 NewLevel);
	int32 GetStatValue_Implementation(ECharacterStatType StatType) const;
	int32 GetStatUpgradeCost_Implementation(ECharacterStatType StatType) const;
	int32 GetAvailableStatPoints_Implementation() const;
	float GetCharacterParam_Implementation(ECharacterParamType ParamType) const;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Fire Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* FireAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AttackAction;
	void Fire(const FInputActionValue& Value);
	void ExecuteDash();
	void Attack(const FInputActionValue& Value);

protected:
	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Resources", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCharacterResourcePool> ManaPool;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spells", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpellCaster> SpellCaster;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dash", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDashComponent> Dash;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dash", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ULevelingComponent> Leveling;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dash", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMeleeCombatComponent> MeleeCombat;
};
