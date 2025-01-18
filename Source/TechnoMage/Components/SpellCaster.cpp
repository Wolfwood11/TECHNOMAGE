#include "SpellCaster.h"
#include "CharacterResourcePool.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "TechnoMage/ObjectPool.h"
#include "TechnoMage/Characters/EnemyCharacter.h"
#include "TechnoMage/Spels/BaseSpell.h"
#include "TechnoMage/Subsystems/ActorTrackingSubsystem.h"

USpellCaster::USpellCaster()
{
	PrimaryComponentTick.bCanEverTick = true;

	CurrentSpellIndex = 0;
	CasterCooldownTimer = 0.0f;
	CasterCooldown = 0.5f; // Кулдаун между завершением стека и началом нового цикла
}

void USpellCaster::BeginPlay()
{
	Super::BeginPlay();

	SpellQueue.Add(0);
	SpellQueue.Add(1);
	SpellQueue.Add(0);

	if (!ManaPool)
	{
		UE_LOG(LogTemp, Warning, TEXT("USpellCaster: ManaPool is not assigned!"));
	}
}

void USpellCaster::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	CasterCooldownTimer -= DeltaTime;
	if (CasterCooldownTimer < 0) CasterCooldownTimer = 0;
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USpellCaster::CastNextSpell()
{
	// Проверяем, можно ли кастовать
	if (CasterCooldownTimer > 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("USpellCaster: Caster is on cooldown!"));
		return;
	}

	// Проверяем валидность индекса
	if (SpellQueue.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("USpellCaster: SpellQueue is empty!"));
		return;
	}


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

	if (CurrentSpellIndex >= SpellQueue.Num())
	{
		CurrentSpellIndex = 0; // Сбрасываем индекс, если дошли до конца очереди
		CasterCooldownTimer = CasterCooldown; // Задаем кулдаун кастера
		return;
	}

	int32 SpellType = SpellQueue[CurrentSpellIndex];
	ABaseSpell* Spell = Cast<ABaseSpell>(objectPool->GetObject(SpellType));

	if (Spell && CanCastSpell(Spell))
	{
		ExecuteSpell(Spell);

		// Устанавливаем кулдаун для заклинания
		CasterCooldownTimer = Spell->GetSpellCooldown();
		CurrentSpellIndex++;
	}
	else if (Spell)
	{
		Spell->ReturnToPool();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("USpellCaster: Failed to cast spell!"));
	}
}

void USpellCaster::SetManaPool(UCharacterResourcePool* manaPoll)
{
	if (!manaPoll)
	{
		UE_LOG(LogTemp, Warning, TEXT("manaPoll is not valid"));
		return;
	}
	ManaPool = manaPoll;
}

AActor* USpellCaster::FindClosestTarget() const
{
	if (!GetOwner())
	{
		return nullptr;
	}

	AActor* Owner = GetOwner();
	FVector OwnerLocation = Owner->GetActorLocation();
	FVector OwnerForward = Owner->GetActorForwardVector();

	// Получаем всех врагов типа AEnemyCharacter
	TArray<AActor*> AllEnemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyCharacter::StaticClass(), AllEnemies);

	AEnemyCharacter* ClosestTarget = nullptr;
	float ClosestDistance = TargetingRadius;

	for (AActor* EnemyActor : AllEnemies)
	{
		AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(EnemyActor);
		if (!Enemy || !Enemy->IsAlive()) // Проверяем, что цель жива
		{
			continue;
		}

		// Вычисляем расстояние до противника
		float DistanceToEnemy = FVector::Dist(OwnerLocation, Enemy->GetActorLocation());
		if (DistanceToEnemy > TargetingRadius)
		{
			continue; // Пропускаем, если цель вне радиуса
		}

		// Вычисляем угол между форвард-вектором игрока и направлением к противнику
		FVector DirectionToEnemy = (Enemy->GetActorLocation() - OwnerLocation).GetSafeNormal();
		float AngleToEnemy = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(OwnerForward, DirectionToEnemy)));

		if (AngleToEnemy > TargetingAngle)
		{
			continue; // Пропускаем, если цель вне угла
		}

		// Проверяем наличие препятствий с помощью LineTrace
		FHitResult HitResult;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(Owner);
		Params.AddIgnoredActor(Enemy);

		bool bHasLineOfSight = !GetWorld()->LineTraceSingleByChannel(
			HitResult,
			OwnerLocation,
			Enemy->GetActorLocation(),
			ECC_Visibility, // Канал коллизии для проверки видимости
			Params
		);

		if (!bHasLineOfSight)
		{
			continue; // Если есть препятствие, пропускаем цель
		}

		// Если всё прошло, проверяем, ближе ли эта цель
		if (DistanceToEnemy < ClosestDistance)
		{
			ClosestDistance = DistanceToEnemy;
			ClosestTarget = Enemy;
		}
	}

	return ClosestTarget;
}

bool USpellCaster::CanCastSpell(ABaseSpell* Spell) const
{
	if (!ManaPool || !Spell)
	{
		return false;
	}

	// Проверяем, достаточно ли маны
	return ManaPool->HasEnoughResource(Spell->GetSpellCost());
}

void USpellCaster::ExecuteSpell(ABaseSpell* Spell)
{
	if (!Spell)
	{
		return;
	}

	FTransform SpawnTransform = GetOwner()->GetActorTransform();
	AEnemyCharacter* Target = Cast<AEnemyCharacter>(FindClosestTarget());

	// Если найдена цель, направляем заклинание в её сторону
	if (Target)
	{
		FVector DirectionToTarget = (Target->GetActorLocation() - SpawnTransform.GetLocation()).GetSafeNormal();
		FRotator TargetRotation = DirectionToTarget.Rotation();
		SpawnTransform.SetRotation(TargetRotation.Quaternion());

		UE_LOG(LogTemp, Log, TEXT("USpellCaster: Casting spell at target %s"), *Target->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("USpellCaster: No target found, casting forward"));
	}

	// Потребляем ману и активируем заклинание
	if (ManaPool->ConsumeResource(Spell->GetSpellCost()))
	{
		Spell->ActivateFromPoll(SpawnTransform, GetOwner());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("USpellCaster: Not enough mana to cast spell!"));
	}
}
