#include "MeleeCombatComponent.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "TimerManager.h"
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
	if (NotifyName == "EnableWeaponCollision")
	{
		EnableWeaponCollision();
	}
	else if (NotifyName == "DisableWeaponCollision")
	{
		DisableWeaponCollision();
	}
}
void UMeleeCombatComponent::PerformAttack(EAttackType AttackType)
{
	if (bIsOnCooldown)
	{
		UE_LOG(LogTemp, Warning, TEXT("Attack is on cooldown!"));
		return;
	}

	if (!CurrentWeapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("No weapon equipped!"));
		return;
	}

	float CooldownTime = CurrentWeapon->AttackSpeedMultiplier;
	if (AttackType == EAttackType::Strong)
	{
		CooldownTime *= 3.0f; // Сильная атака увеличивает кулдаун
	}

	StartCooldown(CooldownTime);

	// Проигрываем анимацию
	if (CurrentWeapon->AttackAnimation)
	{
		AActor* Owner = GetOwner();
		if (Owner)
		{
			UAnimInstance* AnimInstance = Owner->FindComponentByClass<USkeletalMeshComponent>()->GetAnimInstance();
			if (AnimInstance)
			{
				AnimInstance->Montage_Play(CurrentWeapon->AttackAnimation);
			}
		}
	}
}

void UMeleeCombatComponent::EnableWeaponCollision()
{
	if (WeaponCollision)
	{
		WeaponCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		ProcessedTargets.Empty();
	}
}

void UMeleeCombatComponent::DisableWeaponCollision()
{
	if (WeaponCollision)
	{
		WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void UMeleeCombatComponent::OnWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!CurrentWeapon || !OtherActor || ProcessedTargets.Contains(OtherActor) || OtherActor == GetOwner())
	{
		return;
	}

	ProcessedTargets.Add(OtherActor);

	float InitialDamage = CurrentWeapon->Attack;
	FDamageResult Damage = CalculateDamage(InitialDamage, ProcessedTargets.Num() - 1, EAttackType::Normal);

	if (Damage.Damage < 5.0f)
	{
		return; // Не наносим урон, если он слишком мал
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
	DamageResult.Damage = InitialDamage * DamageMultiplier * AttackMultiplier;

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


void UMeleeCombatComponent::StartCooldown(float CooldownTime)
{
	bIsOnCooldown = true;
	GetWorld()->GetTimerManager().SetTimer(AttackCooldownTimer, [this]()
		{
			bIsOnCooldown = false;
		}, CooldownTime, false);
}

bool UMeleeCombatComponent::IsAttackOnCooldown() const
{
	return bIsOnCooldown;
}
