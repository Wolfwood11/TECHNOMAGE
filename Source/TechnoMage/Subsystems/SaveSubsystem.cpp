#include "SaveSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "TechnoMage/Interfaces/SaveableInterface.h"
#include "TechnoMage/SaveSystem/ActorSaveData.h"
#include "TechnoMage/SaveSystem/UMainSaveGame.h"

void USaveSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// Инициализируем объект сохранения
	InitializeSaveGame();
}

void USaveSubsystem::InitializeSaveGame()
{
	if (UGameplayStatics::DoesSaveGameExist(TEXT("DefaultSaveSlot"), 0))
	{
		SaveGameInstance = Cast<UMainSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("DefaultSaveSlot"), 0));
	}
	else
	{
		SaveGameInstance = NewObject<UMainSaveGame>();
	}
}

void USaveSubsystem::SaveActorData(AActor* Actor, const FString& ActorID)
{
	if (!SaveGameInstance || !Actor)
	{
		return;
	}

	// Проверяем, реализует ли актор интерфейс SaveableInterface
	if (Actor->GetClass()->ImplementsInterface(USaveableInterface::StaticClass()))
	{
		FActorSaveData SaveData;
		SaveData.ActorID = ActorID;
		SaveData.ActorTransform = Actor->GetActorTransform();

		// Вызываем интерфейс для сохранения данных
		ISaveableInterface::Execute_SaveData(Actor, SaveData.CustomData);

		SaveGameInstance->SavedActors.Add(SaveData);
	}
}

void USaveSubsystem::LoadActorData(AActor* Actor, const FString& ActorID)
{
	if (!SaveGameInstance || !Actor)
	{
		return;
	}

	// Ищем сохранённые данные для данного актора
	const FActorSaveData* FoundData = SaveGameInstance->SavedActors.FindByPredicate(
		[&ActorID](const FActorSaveData& Data) { return Data.ActorID == ActorID; });

	if (FoundData)
	{
		// Устанавливаем трансформ актора
		Actor->SetActorTransform(FoundData->ActorTransform);

		// Вызываем интерфейс для загрузки данных
		if (Actor->GetClass()->ImplementsInterface(USaveableInterface::StaticClass()))
		{
			ISaveableInterface::Execute_LoadData(Actor, FoundData->CustomData);
		}
	}
}

void USaveSubsystem::SaveGame(const FString& SlotName)
{
	if (SaveGameInstance)
	{
		UGameplayStatics::SaveGameToSlot(SaveGameInstance, SlotName, 0);
	}
}

void USaveSubsystem::LoadGame(const FString& SlotName)
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		SaveGameInstance = Cast<UMainSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
	}
	else
	{
		InitializeSaveGame();
	}
}