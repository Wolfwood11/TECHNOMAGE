#include "CharacterResourcePool.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/BufferArchive.h"
#include "TechnoMage/Interfaces/CharacterGetersInterface.h"
#include "TechnoMage/Interfaces/DamageableInterface.h"

UCharacterResourcePool::UCharacterResourcePool()
{
	PrimaryComponentTick.bCanEverTick = true;

	// Задание начальных значений
	BaseMaxResource = 100.0f;
	MaxResource = 100.0f;

	BaseRegenRate = 5.f;
	RegenRate = 5.f;

	CurrentResource = MaxResource; // На старте ресурс заполнен полностью
	bAllowNegativeResource = false;
	ModifierTypeAffects = EModifierType::Health;
	CharacterParamMaxValue = ECharacterParamType::MaxHpMultiplier;
	CharacterParamRegenRate = ECharacterParamType::HpRegenMultiplier;
}

void UCharacterResourcePool::SaveData_Implementation(TArray<uint8>& OutData)
{
	FBufferArchive Archive;

	// Сохраняем данные
	Archive << CurrentResource;

	OutData = static_cast<TArray<unsigned char>>(Archive);
}

void UCharacterResourcePool::LoadData_Implementation(const TArray<uint8>& InData)
{
	FMemoryReader Reader(InData);

	// Загружаем данные
	Reader << CurrentResource;

	// Логирование для отладки
	UE_LOG(LogTemp, Log, TEXT("CurrentResource: =%f"), CurrentResource);
}


void UCharacterResourcePool::SetAllowNegativeResource(bool newVal)
{
	bAllowNegativeResource = newVal;
}

void UCharacterResourcePool::SetMaxResource(float newVal)
{
	MaxResource = newVal;
}

void UCharacterResourcePool::SetRegenRate(float newVal)
{
	RegenRate = newVal;
}

float UCharacterResourcePool::GetMaxResource() const
{
	return MaxResource;
}

void UCharacterResourcePool::BeginPlay()
{
	Super::BeginPlay();
}

void UCharacterResourcePool::ProcessModifiers(float DeltaTime)
{
	if (GetOwner()->GetClass()->ImplementsInterface(UCharacterGetersInterface::StaticClass()))
	{
		MaxResource = BaseMaxResource * ICharacterGetersInterface::Execute_GetCharacterParam(GetOwner(), CharacterParamMaxValue);
		RegenRate = BaseRegenRate * ICharacterGetersInterface::Execute_GetCharacterParam(GetOwner(), CharacterParamRegenRate);
		TArray<UModifierData*> modifiers = ICharacterGetersInterface::Execute_GetModifiers(GetOwner(), ModifierTypeAffects);

		for (UModifierData* Modifier : modifiers)
		{
			switch (Modifier->OperationType)
			{
			case EModifierOperationType::Add:
				CurrentResource += Modifier->ModifierValue * DeltaTime;
				break;
			case EModifierOperationType::Subtract:
				CurrentResource -= Modifier->ModifierValue * DeltaTime;
				break;
			case EModifierOperationType::Max:
				CurrentResource = MaxResource;
				break;
			case EModifierOperationType::Min:
				CurrentResource = 0;
				break;
			default:;
			}
		}
	}
}

void UCharacterResourcePool::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Восстановление ресурса
	RegenerateResource(DeltaTime);
}

bool UCharacterResourcePool::HasEnoughResource(float Amount) const
{
	return CurrentResource >= Amount;
}

bool UCharacterResourcePool::ConsumeResource(float Amount)
{
	if (bAllowNegativeResource || HasEnoughResource(Amount))
	{
		CurrentResource -= Amount;
		if (CurrentResource < 0.0f && !bAllowNegativeResource)
		{
			CurrentResource = 0.0f;
		}

		// Проверить, достиг ли ресурс 0
		if (CurrentResource <= 0.0f)
		{
			OnResourceDepleted.Broadcast();
		}

		return true;
	}

	return false; // Недостаточно ресурса
}

void UCharacterResourcePool::AddResource(float Amount)
{
	CurrentResource = FMath::Min(CurrentResource + Amount, MaxResource);
}

void UCharacterResourcePool::SetModifierTypeAffects(EModifierType type)
{
	ModifierTypeAffects = type;
}

float UCharacterResourcePool::GetRegenRate() const
{
	return RegenRate;
}

bool UCharacterResourcePool::GetAllowNegativeResource() const
{
	return bAllowNegativeResource;
}

float UCharacterResourcePool::GetCurrentResource() const
{
	return CurrentResource;
}

void UCharacterResourcePool::SetCurrentResource(float NewValue)
{
	CurrentResource = FMath::Clamp(NewValue, bAllowNegativeResource ? -FLT_MAX : 0.0f, MaxResource);

	// Проверить, достиг ли ресурс 0
	if (CurrentResource <= 0.0f)
	{
		OnResourceDepleted.Broadcast();
	}
}

void UCharacterResourcePool::RegenerateResource(float DeltaTime)
{
	ProcessModifiers(DeltaTime);

	if (CurrentResource < MaxResource)
	{
		CurrentResource += RegenRate * DeltaTime;
	}

	CurrentResource = FMath::Clamp(CurrentResource, bAllowNegativeResource ? -FLT_MAX : 0.0f, MaxResource);
}
