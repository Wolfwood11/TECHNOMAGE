#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SaveSubsystem.generated.h"

class UMainSaveGame;

UCLASS()
class TECHNOMAGE_API USaveSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// Инициализация подсистемы
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	// API для сохранения данных актора
	UFUNCTION(BlueprintCallable, Category = "SaveSystem")
	void SaveActorData(AActor* Actor, const FString& ActorID);

	// API для загрузки данных актора
	UFUNCTION(BlueprintCallable, Category = "SaveSystem")
	void LoadActorData(AActor* Actor, const FString& ActorID);

	// API для сохранения всей игры
	UFUNCTION(BlueprintCallable, Category = "SaveSystem")
	void SaveGame(const FString& SlotName);

	// API для загрузки всей игры
	UFUNCTION(BlueprintCallable, Category = "SaveSystem")
	void LoadGame(const FString& SlotName);

private:
	// Ссылка на объект сохранения
	UPROPERTY()
	UMainSaveGame* SaveGameInstance;

	// Инициализация объекта сохранения
	void InitializeSaveGame();
};