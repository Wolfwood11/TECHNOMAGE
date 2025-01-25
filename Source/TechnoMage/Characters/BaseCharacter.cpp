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

}

void ABaseCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	ProcessSpeedModifiers();
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

		if (!IsAlive_Implementation())
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
