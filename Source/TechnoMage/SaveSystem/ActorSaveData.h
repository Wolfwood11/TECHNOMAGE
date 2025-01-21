#pragma once

#include "CoreMinimal.h"
#include "ActorSaveData.generated.h"


USTRUCT(BlueprintType)
struct FComponentSaveData
{
	GENERATED_BODY()

	// Имя компонента
	UPROPERTY()
	FName ComponentName;

	// Данные компонента
	UPROPERTY()
	TArray<uint8> ComponentData;
};
// Структура для хранения данных актора
USTRUCT(BlueprintType)
struct FActorSaveData
{
	GENERATED_BODY()

	// Уникальный идентификатор актора
	UPROPERTY()
	FString ActorID;

	// Трансформ актора
	UPROPERTY()
	FTransform ActorTransform;

	// Данные актора
	UPROPERTY()
	TArray<uint8> CustomData;

	// Данные компонентов
	UPROPERTY()
	TArray<FComponentSaveData> ComponentData;

	FActorSaveData()
		: ActorID(TEXT("")), ActorTransform(FTransform::Identity)
	{
	}
};
