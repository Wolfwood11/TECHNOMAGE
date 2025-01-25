#include "AreaSpell.h"
#include "Kismet/GameplayStatics.h"
#include "TechnoMage/Characters/BaseCharacter.h"
#include "TechnoMage/Characters/BasePlayerCharacter.h"
#include "TechnoMage/Interfaces/DamageableInterface.h"

AAreaSpell::AAreaSpell()
{
	// Устанавливаем параметры по умолчанию
	AreaRadius = 300.0f;
	ActivationTime = 1.5f; // Время перед возвратом в пул
}

void AAreaSpell::BeginPlay()
{
	Super::BeginPlay();
}

void AAreaSpell::SetAreaRadius(float NewRadius)
{
	AreaRadius = NewRadius;
}

void AAreaSpell::PlayActivationEffect()
{
	Super::PlayActivationEffect();

	// Устанавливаем таймер на возврат в пул
	GetWorldTimerManager().SetTimer(ActivationTimer, this, &AAreaSpell::ReturnToPool, ActivationTime, false);

	// Наносим урон по области
	DealDamage();
}

void AAreaSpell::TriggerEffect(const FHitResult& ImpactResult)
{
	// Реализация эффекта столкновения для каждой цели
	if (CollisionEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), CollisionEffect, ImpactResult.ImpactPoint, ImpactResult.ImpactNormal.Rotation(), true);
	}
}

void AAreaSpell::ReturnToPool()
{
	ProcessedActors.Empty();
	Super::ReturnToPool();
}

void AAreaSpell::DealDamage()
{
	TArray<FHitResult> HitResults;
	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(AreaRadius);

	FVector StartLocation = GetActorLocation();

	// Выполняем проверку в радиусе
	bool bHasHit = GetWorld()->SweepMultiByChannel(
		HitResults,
		StartLocation,
		StartLocation,
		FQuat::Identity,
		ECC_WorldDynamic,
		CollisionShape
	);

	if (bHasHit)
	{
		for (const FHitResult& Hit : HitResults)
		{
			AActor* HitActor = Hit.GetActor();

			// Пропускаем уже обработанных акторов
			if (ProcessedActors.Contains(HitActor))
			{
				continue;
			}

			// Пропускаем владельца заклинания
			if (HitActor == GetInstigatorActor())
			{
				continue;
			}

			// Пропускаем игроков
			if (HitActor && HitActor->IsA(ABasePlayerCharacter::StaticClass()))
			{
				continue;
			}

			// Применяем урон только к актёрам с интерфейсом UDamageableInterface
			if (HitActor && HitActor->GetClass()->ImplementsInterface(UDamageableInterface::StaticClass()))
			{
				const auto damage = CalculateDamage();
				IDamageableInterface::Execute_ApplyDamage(HitActor, damage);

				// Добавляем актёра в список обработанных
				ProcessedActors.Add(HitActor);

				// Спавним эффект на каждой цели
				TriggerEffect(Hit);
			}
		}
	}
}

