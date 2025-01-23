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
	if (UGameplayStatics::DoesSaveGameExist(TEXT("DefaultSaveSlot4"), 0))
	{
		SaveGameInstance = Cast<UMainSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("DefaultSaveSlot4"), 0));
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
		// Ищем, существует ли уже запись для данного актора
		FActorSaveData* ExistingSaveData = SaveGameInstance->SavedActors.FindByPredicate(
			[&ActorID](const FActorSaveData& Data) { return Data.ActorID == ActorID; });

		FActorSaveData NewSaveData;
		NewSaveData.ActorID = ActorID;
		NewSaveData.ActorTransform = Actor->GetActorTransform();

		// Вызываем интерфейс для сохранения данных актора
		ISaveableInterface::Execute_SaveData(Actor, NewSaveData.CustomData);

		// Сохраняем данные компонентов
		for (UActorComponent* Component : Actor->GetComponents())
		{
			if (Component->GetClass()->ImplementsInterface(USaveableInterface::StaticClass()))
			{
				FComponentSaveData ComponentSaveData;

				// Сохраняем имя компонента
				ComponentSaveData.ComponentName = Component->GetFName();

				// Сохраняем данные компонента
				ISaveableInterface::Execute_SaveData(Component, ComponentSaveData.ComponentData);

				NewSaveData.ComponentData.Add(ComponentSaveData);
			}
		}

		if (ExistingSaveData)
		{
			// Обновляем существующие данные
			*ExistingSaveData = NewSaveData;
		}
		else
		{
			// Добавляем новые данные
			SaveGameInstance->SavedActors.Add(NewSaveData);
		}
	}

	SaveGame("DefaultSaveSlot4");
}


void USaveSubsystem::LoadActorData(AActor* Actor, const FString& ActorID)
{
	LoadGame("DefaultSaveSlot4");

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

		// Вызываем интерфейс для загрузки данных актора
		if (Actor->GetClass()->ImplementsInterface(USaveableInterface::StaticClass()))
		{
			ISaveableInterface::Execute_LoadData(Actor, FoundData->CustomData);
		}

		// Загружаем данные компонентов
		TMap<FName, UActorComponent*> ComponentMap;
		for (UActorComponent* Component : Actor->GetComponents())
		{
			if (Component)
			{
				ComponentMap.Add(Component->GetFName(), Component);
			}
		}

		for (const FComponentSaveData& ComponentSaveData : FoundData->ComponentData)
		{
			if (UActorComponent** FoundComponent = ComponentMap.Find(ComponentSaveData.ComponentName))
			{
				if ((*FoundComponent)->GetClass()->ImplementsInterface(USaveableInterface::StaticClass()))
				{
					ISaveableInterface::Execute_LoadData(*FoundComponent, ComponentSaveData.ComponentData);
				}
			}
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