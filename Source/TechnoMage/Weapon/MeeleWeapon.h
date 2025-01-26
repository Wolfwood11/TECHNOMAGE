#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Animation/AnimMontage.h"
#include "TechnoMage/Modificators/ModifierData.h"
#include "MeeleWeapon.generated.h"

UCLASS(Blueprintable, BlueprintType)
class TECHNOMAGE_API UMeeleWeapon : public UDataAsset
{
	GENERATED_BODY()

public:
	// Атака оружия
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	float Attack = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	float MaxAttack = 150.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats", meta = (AllowPrivateAccess = "true"))
	TArray<UModifierData*> Modifiers;

	// Модификатор скорости атаки
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	float AttackSpeedMultiplier = 1.0f;

	// Дистанция атаки
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	float AttackRange = 150.0f;

	// Требования к силе
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Requirements")
	int32 RequiredStrength = 10;

	// Требования к уровню
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Requirements")
	int32 RequiredLevel = 1;

	// Иконка оружия
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
	TObjectPtr<UTexture2D> Icon;

	// Модель оружия
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
	TObjectPtr<UStaticMesh> WeaponModel;

	// Анимация атаки
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TObjectPtr<UAnimMontage> AttackAnimation;

	// Партикл-эффект на оружии
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	TObjectPtr<UParticleSystem> WeaponParticleEffect;

	// Партикл-эффект удара
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	TObjectPtr<UParticleSystem> HitEffect;

	// Звук удара
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	TObjectPtr<USoundCue> HitSound;

	// Звук замаха
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	TObjectPtr<USoundCue> SwingSound;
};
