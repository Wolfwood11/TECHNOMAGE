#include "BaseSpell.h"

#include "Kismet/GameplayStatics.h"
#include "TechnoMage/Interfaces/DamageableInterface.h"

ABaseSpell::ABaseSpell()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create Capsule Component
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	RootComponent = CapsuleComponent;
	CapsuleComponent->InitCapsuleSize(50.f, 50.f);

	// Setup Collision
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CapsuleComponent->SetCollisionObjectType(ECC_WorldDynamic);
	CapsuleComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	CapsuleComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	CapsuleComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &ABaseSpell::OnCapsuleOverlap);

	// Create Particle System Component
	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));
	ParticleSystemComponent->SetupAttachment(CapsuleComponent);
	ParticleSystemComponent->SetAutoActivate(false);
}

void ABaseSpell::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseSpell::UpdateComponentsState(bool value)
{
	Super::UpdateComponentsState(value);
	if (CapsuleComponent)
	{
		CapsuleComponent->SetComponentTickEnabled(value);
	}
}

void ABaseSpell::ReturnToPool()
{
	if (ParticleSystemComponent)
	{
		ParticleSystemComponent->Deactivate();
	}
	Super::ReturnToPool();
}

void ABaseSpell::ActivateFromPoll(const FTransform& transform, AActor* NewInstigatorActor)
{
	Super::ActivateFromPoll(transform, NewInstigatorActor);
	if (ParticleSystemComponent)
	{
		ParticleSystemComponent->Activate(true);
	}
	PlayActivationEffect();
}

void ABaseSpell::AddModifier(UModifierData* modifier)
{
	if (!modifier) return;
	modifiers.Add(modifier);
}

void ABaseSpell::RemoveModifier(UModifierData* modifier)
{
	if (!modifier) return;

	int32 Index = modifiers.Find(modifier);
	if (Index != INDEX_NONE)
	{
		modifiers.RemoveAt(Index);
	}
}

int ABaseSpell::GetSpellCost() const
{
	return SpellCost;
}

float ABaseSpell::GetSpellCooldown() const
{
	return SpellCooldown;
}

float ABaseSpell::GetDamage() const
{
	return Damage;
}

float ABaseSpell::GetMaxDamage() const
{
	return MaxDamage;
}

float ABaseSpell::GetCriticalChance() const
{
	return CriticalChance;
}

ESpellElement ABaseSpell::GetSpellElement() const
{
	return Element;
}

TArray<UModifierData*> ABaseSpell::GetModifiers() const
{
	return modifiers;
}

FDamageResult ABaseSpell::CalculateDamage() const
{
	FDamageResult Result;

	float RandomChance = FMath::FRandRange(0.0f, 100.0f);
	if (RandomChance <= CriticalChance)
	{
		Result.bIsCritical = true;
		Result.Damage = MaxDamage * 1.5f;
	}
	else
	{
		Result.Damage = FMath::FRandRange(Damage, MaxDamage);
	}

	Result.Element = Element;

	Result.Modifiers = modifiers;

	return Result;
}

void ABaseSpell::OnCapsuleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == GetOwner() || OtherActor == GetInstigatorActor())
	{
		return;
	}

	if (OtherActor->GetClass()->ImplementsInterface(UDamageableInterface::StaticClass()))
	{
		if (IDamageableInterface* DamageableActor = Cast<IDamageableInterface>(OtherActor))
		{
			const auto damage = CalculateDamage();
			DamageableActor->ApplyDamage(damage);
		}
	}

	// Вызов эффекта столкновения
	TriggerEffect(SweepResult);
}

void ABaseSpell::TriggerEffect(const FHitResult& ImpactResult)
{
	if (CollisionEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), CollisionEffect, ImpactResult.ImpactPoint, ImpactResult.ImpactNormal.Rotation(), true);
	}
}

void ABaseSpell::PlayActivationEffect()
{
	if (ActivationEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ActivationEffect, GetActorLocation(), GetActorRotation(), true);
	}
}
