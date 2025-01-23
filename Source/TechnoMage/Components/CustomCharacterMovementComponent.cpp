#include "CustomCharacterMovementComponent.h"

#include "StatsModifiersComponent.h"
#include "GameFramework/Actor.h"
#include "TechnoMage/Interfaces/CharacterGetersInterface.h"

UCustomCharacterMovementComponent::UCustomCharacterMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCustomCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	OriginalMaxWalkSpeed = MaxWalkSpeed;
}

void UCustomCharacterMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Применяем модификаторы скорости
	ApplySpeedModifiers();
}

void UCustomCharacterMovementComponent::ApplySpeedModifiers()
{
	AActor* OwnerActor = GetOwner();
	if (!OwnerActor || !OwnerActor->GetClass()->ImplementsInterface(UCharacterGetersInterface::StaticClass()))
	{
		return;
	}

	// Получаем модификаторы скорости через интерфейс
	TArray<UModifierData*> Modifiers = ICharacterGetersInterface::Execute_GetModifiers(OwnerActor, EModifierType::Speed);
	float speedMultiplier = FMath::Max(1.f, ICharacterGetersInterface::Execute_GetCharacterParam(OwnerActor, ECharacterParamType::Velocity));
	// Применяем модификаторы
	float ModifiedSpeed = OriginalMaxWalkSpeed * speedMultiplier;
	UStatsModifiersComponent::ProcessModifiers(Modifiers, ModifiedSpeed);
	// Устанавливаем изменённую скорость
	MaxWalkSpeed = FMath::Max(ModifiedSpeed, 0.0f); // Скорость не может быть отрицательной
}
