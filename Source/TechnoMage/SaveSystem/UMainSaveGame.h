#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ActorSaveData.h" // Структура для хранения данных акторов
#include "UMainSaveGame.generated.h"

// Главный класс для сохранения
UCLASS(Blueprintable)
class TECHNOMAGE_API UMainSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UMainSaveGame();

	// Слот для сохранения
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SaveGame")
	FString SaveSlotName;

	// Индекс пользователя (по умолчанию 0)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SaveGame")
	int32 UserIndex;

	// Сохранённые данные игрока
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SaveGame")
	FActorSaveData PlayerData;

	// Сохранённые данные акторов
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SaveGame")
	TArray<FActorSaveData> SavedActors;
};
