#include "BaseCharacter.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TechnoMage/ObjectPool.h"
#include "TechnoMage/Components/CharacterResourcePool.h"
#include "TechnoMage/Components/CustomCharacterMovementComponent.h"
#include "TechnoMage/Subsystems/ActorTrackingSubsystem.h"
#include "TechnoMage/UIActors/DamageNumberActor.h"

ABaseCharacter::ABaseCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UCustomCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	CurrentLockComponent = nullptr;
	CurrentLockTime = 0.f;
	PrimaryActorTick.bCanEverTick = true;
	// Настраиваем коллизию капсулы
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f); // Радиус и высота капсулы
	GetCapsuleComponent()->SetCollisionObjectType(ECC_Pawn);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);

	HealthPool = CreateDefaultSubobject<UCharacterResourcePool>(TEXT("HealthPool"));
	if (HealthPool)
	{
		HealthPool->SetAllowNegativeResource(true);
	}

	StatsModifiersComponent = CreateDefaultSubobject<UStatsModifiersComponent>(TEXT("StatsModifiers"));
}

void ABaseCharacter::BeginPlay()
{
	if (GetCharacterMovement())
	{
		maxSpeed = GetCharacterMovement()->GetMaxSpeed();
	}
	CurrentLockComponent = nullptr;
	CurrentLockTime = 0.f;
	CurrentDisableMovement = false;
	Super::BeginPlay();
}

void ABaseCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ABaseCharacter::ProcessSpeedModifiers() const
{

}

void ABaseCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	ProcessSpeedModifiers();

	if (CurrentLockComponent != nullptr)
	{
		CurrentLockTime -= DeltaSeconds;
		if (CurrentLockTime <= 0.f)
		{
			UnlockActions();
		}
	}
}

void ABaseCharacter::ApplyDamage_Implementation(const FDamageResult& damageResult)
{
	if (HealthPool)
	{
		const auto Damage = damageResult.Damage;
		HealthPool->ConsumeResource(Damage);

		TObjectPtr<AObjectPool> ObjectPool = nullptr;
		if (TObjectPtr<UActorTrackingSubsystem> Subsystem = GetWorld()->GetSubsystem<UActorTrackingSubsystem>())
		{
			ObjectPool = Subsystem->GetRegisteredActor();
		}

		if (!ObjectPool)
		{
			UE_LOG(LogTemp, Warning, TEXT("USpellCaster: ObjectPool is empty!"));
			return;
		}

		if (TObjectPtr<ADamageNumberActor> DamageNumber = Cast<ADamageNumberActor>(ObjectPool->GetObject(ADamageNumberActor::StaticClass())))
		{
			FTransform DamageTextTransform(
				FQuat::Identity,                       // Поворот сброшен
				GetActorLocation(),                    // Позиция на 200 единиц выше актора
				FVector(1.0f)                          // Масштаб по умолчанию
			);
			DamageNumber->Initialize(Damage, DamageTextTransform, this, damageResult.bIsCritical);
		}

		bool bIsTargetAlive = Execute_IsAlive(this);
		if (!bIsTargetAlive)
		{
			Die();
			return;
		}

		if (StatsModifiersComponent)
		{
			StatsModifiersComponent->ApplyModifier(damageResult);
		}
	}
	else
	{
		Die();
	}
}

bool ABaseCharacter::IsAlive_Implementation() const
{
	return HealthPool ? HealthPool->GetCurrentResource() > 0 : false;
}

void ABaseCharacter::Die()
{
	Destroy();
}

TArray<UModifierData*> ABaseCharacter::GetModifiers_Implementation(EModifierType Type) const
{
	return StatsModifiersComponent ? StatsModifiersComponent->GetModifiers(Type) : TArray<UModifierData*>();
}

float ABaseCharacter::GetHealth_Implementation() const
{
	return HealthPool ? HealthPool->GetCurrentResource() : 0.0f;
}

float ABaseCharacter::GetMana_Implementation() const
{
	return 1.0f;
}

float ABaseCharacter::GetMaxHealth_Implementation() const
{
	return HealthPool ? HealthPool->GetMaxResource() : 1.0f;
}

float ABaseCharacter::GetMaxMana_Implementation() const
{
	return 1.0f;
}

void ABaseCharacter::AddExp_Implementation(int exp)
{
	ICharacterSettersInterface::AddExp_Implementation(exp);
}

bool ABaseCharacter::Lock_Implementation(TSubclassOf<UActorComponent> ComponentClass, bool DisableMovement)
{
	if (!ComponentClass)
	{
		return false;
	}

	const float CurrentTime = GetWorld()->GetTimeSeconds();

	if (CurrentLockComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Action already locked by %s, cannot lock with %s"), *CurrentLockComponent->GetName(), *ComponentClass->GetName());
		return false;
	}

	CurrentLockComponent = ComponentClass;
	CurrentLockTime = maxLockTime;
	CurrentDisableMovement = DisableMovement;

	if (DisableMovement)
	{
		if (UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
		{
			MovementComponent->StopMovementImmediately();
			MovementComponent->DisableMovement();
		}
	}
	return true;
}

void ABaseCharacter::UnlockActions()
{
	CurrentLockComponent = nullptr;
	CurrentLockTime = 0.0f;
	if (CurrentDisableMovement)
	{
		CurrentDisableMovement = false;
		if (GetCharacterMovement())
		{
			GetCharacterMovement()->SetMovementMode(MOVE_Walking);
			UE_LOG(LogTemp, Log, TEXT("Movement re-enabled for character."));
		}
	}
}

bool ABaseCharacter::UnLock_Implementation(TSubclassOf<UActorComponent> ComponentClass)
{
	if (!ComponentClass || CurrentLockComponent != ComponentClass)
	{
		return false;
	}

	UnlockActions();

	UE_LOG(LogTemp, Log, TEXT("Action unlocked by %s"), *ComponentClass->GetName());
	return true;
}

bool ABaseCharacter::IsLocked_Implementation() const
{
	return CurrentLockComponent != nullptr;
}

bool ABaseCharacter::IsLockedByMe_Implementation(TSubclassOf<UActorComponent> ComponentClass) const
{
	if (ComponentClass && CurrentLockComponent == ComponentClass)
	{
		return true;
	}
	return false;
}