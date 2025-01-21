#include "BaseCharacter.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TechnoMage/ObjectPool.h"
#include "TechnoMage/Components/CharacterResourcePool.h"
#include "TechnoMage/Subsystems/ActorTrackingSubsystem.h"
#include "TechnoMage/UIActors/DamageNumberActor.h"

ABaseCharacter::ABaseCharacter()
{
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
	Super::BeginPlay();
}

void ABaseCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ABaseCharacter::ProcessSpeedModifiers() const
{
	if (maxSpeed <= 0)
	{
		check(false);
		return;
	}

	float CurrentMaxSpeed = maxSpeed;
	TArray<UModifierData*> modifiers = GetModifiers_Implementation(EModifierType::Speed);
	for (UModifierData* Modifier : modifiers)
	{
		switch (Modifier->OperationType)
		{
		case EModifierOperationType::Add:
			CurrentMaxSpeed += Modifier->ModifierValue;
			break;
		case EModifierOperationType::Subtract:
			CurrentMaxSpeed -= Modifier->ModifierValue;
			break;
		case EModifierOperationType::Max:
			CurrentMaxSpeed = maxSpeed;
			break;
		case EModifierOperationType::Multiply:
			CurrentMaxSpeed *= Modifier->ModifierValue;
			break;
		case EModifierOperationType::Divide:
			CurrentMaxSpeed /= Modifier->ModifierValue;
			break;
		case EModifierOperationType::Replace:
			CurrentMaxSpeed = Modifier->ModifierValue;
			break;
		case EModifierOperationType::Min:
			CurrentMaxSpeed = 0;
			break;
		default:;
		}
	}

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = FMath::Clamp(CurrentMaxSpeed, 0, 2 * maxSpeed);
	}
}

void ABaseCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	ProcessSpeedModifiers();
}

void ABaseCharacter::ApplyDamage(const FDamageResult& damageResult)
{
	if (HealthPool)
	{
		const auto Damage = damageResult.Damage;
		HealthPool->ConsumeResource(Damage);

		AObjectPool* objectPool = nullptr;
		if (UActorTrackingSubsystem* Subsystem = GetWorld()->GetSubsystem<UActorTrackingSubsystem>())
		{
			objectPool = Subsystem->GetRegisteredActor();
		}

		if (!objectPool)
		{
			UE_LOG(LogTemp, Warning, TEXT("USpellCaster: ObjectPool is empty!"));
			return;
		}

		if (ADamageNumberActor* DamageNumber = Cast<ADamageNumberActor>(objectPool->GetObject(ADamageNumberActor::StaticClass())))
		{
			FTransform DamageTextTransform(
				FQuat::Identity,                       // Поворот сброшен
				GetActorLocation(), // Позиция на 200 единиц выше актора
				FVector(1.0f)                          // Масштаб по умолчанию
			);
			DamageNumber->Initialize(Damage, DamageTextTransform, this, damageResult.bIsCritical);
		}

		if (!IsAlive())
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

bool ABaseCharacter::IsAlive() const
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
	ICharacterGetersInterface::AddExp_Implementation(exp);
}
