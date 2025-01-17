#pragma once

#include "CoreMinimal.h"
#include "ActorSaveData.generated.h"


// Структура для хранения данных актора
USTRUCT(BlueprintType)
struct FActorSaveData
{
	GENERATED_BODY()

	// Уникальный идентификатор актора
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SaveData")
	FString ActorID;

	// Трансформ актора
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SaveData")
	FTransform ActorTransform;

	// Дополнительные данные актора
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SaveData")
	TArray<uint8> CustomData;

	FActorSaveData()
		: ActorID(TEXT("")), ActorTransform(FTransform::Identity)
	{
	}
};
