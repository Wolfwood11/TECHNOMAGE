#include "MeleeCombatComponent.h"

#include "DashComponent.h"
#include "StatsModifiersComponent.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "TechnoMage/Animations/TrailManager.h"
#include "TechnoMage/Interfaces/ActionLockInterface.h"
#include "TechnoMage/Interfaces/CharacterGetersInterface.h"
#include "TechnoMage/Interfaces/DamageableInterface.h"
#include "TechnoMage/Weapon/MeeleWeapon.h"

UMeleeCombatComponent::UMeleeCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// Создаём компонент статического меша для меча
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->SetVisibility(false);

	// Создаём компонент коллизии для меча
	WeaponCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("WeaponCollision"));
	WeaponCollision->InitCapsuleSize(10.0f, 50.0f); // Размер капсулы по умолчанию
	WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponCollision->SetCollisionObjectType(ECC_WorldDynamic);
	WeaponCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	WeaponCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	WeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &UMeleeCombatComponent::OnWeaponOverlap);

	TrailManager = CreateDefaultSubobject<UTrailManager>(TEXT("TrailManager2"));
	if (TrailManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("TrailManager"));
	}
}

void UMeleeCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	AttachWeaponToSocket();

	if (WeaponMesh)
	{
		WeaponMesh->AttachToComponent(GetOwner()->FindComponentByClass<USkeletalMeshComponent>(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocketName);
	}

	if (WeaponCollision)
	{
		WeaponCollision->AttachToComponent(GetOwner()->FindComponentByClass<USkeletalMeshComponent>(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocketName);
		WeaponCollision->SetRelativeLocation(FVector(0.0f, -60.0f, 0.0f));
		WeaponCollision->SetRelativeRotation(FRotator(0.0f, 0.0f, 90.0f));
	}

	UAnimInstance* AnimInstance = GetOwner()->FindComponentByClass<USkeletalMeshComponent>()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &UMeleeCombatComponent::HandleAnimationNotify);
		AnimInstance->OnMontageEnded.AddDynamic(this, &UMeleeCombatComponent::OnMontageEnded);
	}
}

void UMeleeCombatComponent::EquipWeapon(UMeeleWeapon* NewWeapon)
{
	if (!NewWeapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("No weapon equipped!"));
		return;
	}

	CurrentWeapon = NewWeapon;

	// Обновляем статический меш и коллизию
	AttachWeaponToSocket();
}

void UMeleeCombatComponent::AttachWeaponToSocket()
{
	if (!CurrentWeapon || !GetOwner())
	{
		return;
	}

	// Устанавливаем статический меш меча
	if (WeaponMesh && CurrentWeapon->WeaponModel)
	{
		WeaponMesh->SetStaticMesh(CurrentWeapon->WeaponModel);
		WeaponMesh->SetVisibility(true);
	}

	// Подстраиваем размеры капсулы под меч
	AdjustCollisionSize();

	if (!TrailManager)
	{
		TrailManager = NewObject<UTrailManager>(this, UTrailManager::StaticClass());
		if (TrailManager)
		{
			UE_LOG(LogTemp, Warning, TEXT("TrailManager создан в BeginPlay"));
			TrailManager->RegisterComponent();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Ошибка создания TrailManager"));
		}
	}

	if (WeaponMesh && CurrentWeapon->WeaponParticleEffect && TrailManager)
	{
		TrailManager->Initialize(CurrentWeapon->WeaponParticleEffect, FName("trailStart"), FName("trailEnd"));
	}
}

void UMeleeCombatComponent::AdjustCollisionSize()
{
	if (!WeaponCollision || !CurrentWeapon)
	{
		return;
	}

	// Подгоняем длину и ширину капсулы под размеры меча
	float CapsuleRadius = FMath::Clamp(CurrentWeapon->AttackRange * 0.1f, 5.0f, 50.0f);
	float CapsuleHalfHeight = CurrentWeapon->AttackRange * 0.5f;

	WeaponCollision->SetCapsuleSize(CapsuleRadius, CapsuleHalfHeight);
}

void UMeleeCombatComponent::HandleAnimationNotify(FName NotifyName, const FBranchingPointNotifyPayload& Payload)
{
	if (GetOwner() && GetOwner()->GetClass()->ImplementsInterface(UActionLockInterface::StaticClass()))
	{
		if (!IActionLockInterface::Execute_IsLockedByMe(GetOwner(), UMeleeCombatComponent::StaticClass()))
		{
			return;
		}
	}

	if (NotifyName == "EnableWeaponCollision")
	{
		EnableWeaponCollision();
		if (CurrentWeapon && CurrentWeapon->SwingSound)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), CurrentWeapon->SwingSound, GetOwner()->GetActorLocation());
		}
		TrailManager->StartTrail(WeaponMesh);
	}
	else if (NotifyName == "DisableWeaponCollision")
	{
		DisableWeaponCollision();
		TrailManager->StopTrail();
	}
}

void UMeleeCombatComponent::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (GetOwner() && GetOwner()->GetClass()->ImplementsInterface(UActionLockInterface::StaticClass()))
	{
		if (!IActionLockInterface::Execute_IsLockedByMe(GetOwner(), UMeleeCombatComponent::StaticClass()))
		{
			return;
		}
	}

	if (Montage == CurrentWeapon->AttackAnimation)
	{
		GetWorld()->GetTimerManager().SetTimer(AttackCooldownTimer, [this]()
			{
				CurrentCooldownTime = 0.f;
			}, CurrentCooldownTime, false);

		UE_LOG(LogTemp, Log, TEXT("Attack animation montage ended. Interrupted: %s"), bInterrupted ? TEXT("True") : TEXT("False"));
		if (GetOwner() && GetOwner()->GetClass()->ImplementsInterface(UActionLockInterface::StaticClass()))
		{
			IActionLockInterface::Execute_UnLock(GetOwner(), UMeleeCombatComponent::StaticClass());
		}
		// Снимаем блокировку или выполняем другие действия
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Different montage ended."));
	}
}

float UMeleeCombatComponent::CalculateAttackSpeedMultiplier()
{
	AActor* OwnerActor = GetOwner();
	if (!OwnerActor || !OwnerActor->GetClass()->ImplementsInterface(UCharacterGetersInterface::StaticClass()))
	{
		return 1.f;
	}

	// Получаем модификаторы скорости через интерфейс
	float CooldownMultiplier = FMath::Max(1.f, ICharacterGetersInterface::Execute_GetCharacterParam(OwnerActor, ECharacterParamType::AttackSpeed));
	TArray<UModifierData*> Modifiers = ICharacterGetersInterface::Execute_GetModifiers(OwnerActor, EModifierType::Speed);
	float EffectModifier = 1.f;
	UStatsModifiersComponent::ProcessModifiers(Modifiers, EffectModifier);
	CooldownMultiplier = EffectModifier > 0 ? CooldownMultiplier / EffectModifier : CooldownMultiplier;
	return CooldownMultiplier;
}

void UMeleeCombatComponent::PerformAttack(EAttackType AttackType)
{
	if (GetOwner() && GetOwner()->GetClass()->ImplementsInterface(UActionLockInterface::StaticClass()))
	{
		if (IActionLockInterface::Execute_IsLocked(GetOwner()))
		{
			return;
		}
	}

	if (IsAttackOnCooldown())
	{
		UE_LOG(LogTemp, Warning, TEXT("Attack is on cooldown!"));
		return;
	}

	if (!CurrentWeapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("No weapon equipped!"));
		return;
	}
	const float attackSpeedMultiplier = CalculateAttackSpeedMultiplier();
	float CooldownTime = BaseCoolDownTime * CurrentWeapon->AttackSpeedMultiplier / attackSpeedMultiplier;
	if (AttackType == EAttackType::Strong)
	{
		CooldownTime *= 3.0f; // Сильная атака увеличивает кулдаун
	}

	SetupCooldown(CooldownTime);

	// Проигрываем анимацию
	if (CurrentWeapon->AttackAnimation)
	{
		AActor* Owner = GetOwner();
		if (Owner)
		{
			UAnimInstance* AnimInstance = Owner->FindComponentByClass<USkeletalMeshComponent>()->GetAnimInstance();
			if (AnimInstance)
			{
				AnimInstance->Montage_Play(CurrentWeapon->AttackAnimation, attackSpeedMultiplier);
				if (GetOwner() && GetOwner()->GetClass()->ImplementsInterface(UActionLockInterface::StaticClass()))
				{
					IActionLockInterface::Execute_Lock(GetOwner(), UMeleeCombatComponent::StaticClass(), true);
				}
			}
		}
	}
}

void UMeleeCombatComponent::EnableWeaponCollision()
{
	ProcessedTargets.Empty();
	if (WeaponCollision)
	{
		WeaponCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}

void UMeleeCombatComponent::DisableWeaponCollision()
{
	ProcessedTargets.Empty();
	if (WeaponCollision)
	{
		WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void UMeleeCombatComponent::OnWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Проверяем базовые условия
	if (!CurrentWeapon || !OtherActor || OtherActor == GetOwner())
	{
		return;
	}

	// Проверяем, если цель уже была обработана
	if (ProcessedTargets.Contains(OtherActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("Already processed target: %s"), *OtherActor->GetName());
		return;
	}

	ProcessedTargets.Add(OtherActor);

	float InitialDamage = CurrentWeapon->Attack;
	FDamageResult Damage = CalculateDamage(InitialDamage, ProcessedTargets.Num() - 1, EAttackType::Normal);

	if (Damage.Damage < 5.0f)
	{
		return; // Не наносим урон, если он слишком мал
	}

	// Воспроизвести эффект удара
	if (CurrentWeapon->HitEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), CurrentWeapon->HitEffect, SweepResult.ImpactPoint);
	}

	// Воспроизвести звук удара
	if (CurrentWeapon->HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), CurrentWeapon->HitSound, SweepResult.ImpactPoint);
	}

	if (OtherActor->GetClass()->ImplementsInterface(UDamageableInterface::StaticClass()))
	{
		IDamageableInterface::Execute_ApplyDamage(OtherActor, Damage);
	}
}

FDamageResult UMeleeCombatComponent::CalculateDamage(float InitialDamage, int TargetIndex, EAttackType AttackType) const
{
	FDamageResult DamageResult;

	// Уменьшение урона для последующих целей
	float DamageMultiplier = FMath::Pow(0.8f, TargetIndex);

	// Увеличение урона для сильной атаки
	float AttackMultiplier = (AttackType == EAttackType::Strong) ? 2.0f : 1.0f;

	// Базовый урон
	AActor* OwnerActor = GetOwner();
	if (!OwnerActor || !OwnerActor->GetClass()->ImplementsInterface(UCharacterGetersInterface::StaticClass()))
	{
		return DamageResult;
	}

	const float paramsMultiplier = FMath::Max(1.f, ICharacterGetersInterface::Execute_GetCharacterParam(OwnerActor, ECharacterParamType::AtkMultiplier));
	DamageResult.Damage = InitialDamage * DamageMultiplier * AttackMultiplier * paramsMultiplier;

	// Проверка на критический удар
	float CriticalChance = 10.f;
	float RandomRoll = FMath::FRandRange(0.0f, 100.0f);

	if (RandomRoll <= CriticalChance)
	{
		DamageResult.bIsCritical = true;
		DamageResult.Damage *= 1.5f; // Увеличиваем урон при критическом ударе
	}
	else
	{
		DamageResult.bIsCritical = false;
	}

	// Назначение элемента атаки
	DamageResult.Element = ESpellElement::Normal;

	// Модификаторы атаки
	if (CurrentWeapon)
	{
		DamageResult.Modifiers = CurrentWeapon->Modifiers;
	}

	return DamageResult;
}


void UMeleeCombatComponent::SetupCooldown(float CooldownTime)
{
	CurrentCooldownTime = CooldownTime;
}

bool UMeleeCombatComponent::IsAttackOnCooldown() const
{
	return CurrentCooldownTime > 0;
}
