#include "DashComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "CharacterResourcePool.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TechnoMage/Interfaces/ActionLockInterface.h"

UDashComponent::UDashComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	DashDistance = 1000.0f;
	Cooldown = 3.0f;
	ManaCost = 20.0f;
	DashDuration = 0.1f; // Рывок занимает 0.1 секунды
	LastDashTime = -Cooldown;

	bIsDashing = false;
	DashElapsedTime = 0.0f;
}

void UDashComponent::SetManaPool(UCharacterResourcePool* manaPoll)
{

	if (!manaPoll)
	{
		UE_LOG(LogTemp, Warning, TEXT("manaPoll is not valid"));
		return;
	}
	ManaPool = manaPoll;
}

void UDashComponent::SetDashEffect(UParticleSystemComponent* effect)
{
	DashEffectComponent = effect;
}

void UDashComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UDashComponent::PerformDash()
{
	if (!CanDash())
	{
		return;
	}

	// Потребляем ману
	if (ManaPool && !ManaPool->ConsumeResource(ManaCost))
	{
		UE_LOG(LogTemp, Warning, TEXT("UDashComponent: Not enough mana for dash!"));
		return;
	}

	// Воспроизводим эффект рывка
	PlayDashEffect();

	// Запускаем рывок
	StartDash();

	// Устанавливаем время последнего рывка
	LastDashTime = GetWorld()->GetTimeSeconds();
}

bool UDashComponent::DashInPrecess() const
{
	return bIsDashing;
}


bool UDashComponent::CanDash() const
{
	if (bIsDashing)
	{
		return false;
	}

	if (GetWorld()->GetTimeSeconds() - LastDashTime < Cooldown)
	{
		UE_LOG(LogTemp, Warning, TEXT("UDashComponent: Dash is on cooldown!"));
		return false;
	}

	if (!ManaPool || !ManaPool->HasEnoughResource(ManaCost))
	{
		UE_LOG(LogTemp, Warning, TEXT("UDashComponent: Not enough mana!"));
		return false;
	}

	if (GetOwner() && GetOwner()->GetClass()->ImplementsInterface(UActionLockInterface::StaticClass()))
	{
		if (IActionLockInterface::Execute_IsLocked(GetOwner()))
		{
			return false;
		}
	}

	return true;
}

void UDashComponent::PlayDashEffect()
{
	if (DashEffectComponent && !DashEffectComponent->IsActive())
	{
		DashEffectComponent->ActivateSystem();
	}
}

void UDashComponent::StartDash()
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!OwnerCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("UDashComponent: Owner is not a character!"));
		return;
	}

	// Устанавливаем начальную позицию, направление и конечную точку
	StartLocation = OwnerCharacter->GetActorLocation();
	DashDirection = OwnerCharacter->GetActorForwardVector();
	EndLocation = StartLocation + DashDirection * DashDistance;

	// Проверка на столкновения
	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(OwnerCharacter);
	Params.bTraceComplex = true;

	bool bHit = GetWorld()->SweepSingleByChannel(
		Hit,
		StartLocation,
		EndLocation,
		FQuat::Identity,
		ECC_Visibility,
		FCollisionShape::MakeCapsule(34.0f, 88.0f),
		Params
	);

	if (bHit)
	{
		EndLocation = Hit.Location;
	}

	// Активируем движение
	bIsDashing = true;
	DashElapsedTime = 0.0f;

	if (GetOwner() && GetOwner()->GetClass()->ImplementsInterface(UActionLockInterface::StaticClass()))
	{
		IActionLockInterface::Execute_Lock(GetOwner(), UDashComponent::StaticClass(), true);
	}

	UE_LOG(LogTemp, Log, TEXT("UDashComponent: Dash started!"));
}

void UDashComponent::EndDash()
{
	bIsDashing = false;
	// Останавливаем эффект
	if (DashEffectComponent && DashEffectComponent->IsActive())
	{
		DashEffectComponent->DeactivateSystem();
	}

	if (GetOwner() && GetOwner()->GetClass()->ImplementsInterface(UActionLockInterface::StaticClass()))
	{
		IActionLockInterface::Execute_UnLock(GetOwner(), UDashComponent::StaticClass());
	}
	UE_LOG(LogTemp, Log, TEXT("UDashComponent: Dash ended!"));
}

void UDashComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsDashing)
	{
		DashElapsedTime += DeltaTime;
		float Alpha = FMath::Clamp(DashElapsedTime / DashDuration, 0.0f, 1.0f);

		// Линейная интерполяция позиции
		FVector NewLocation = FMath::Lerp(StartLocation, EndLocation, Alpha);

		if (ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()))
		{
			OwnerCharacter->SetActorLocation(NewLocation, false, nullptr, ETeleportType::TeleportPhysics);
		}

		// Завершаем рывок
		if (DashElapsedTime >= DashDuration)
		{
			EndDash();
		}
	}
}
