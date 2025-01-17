#include "TechnoMageSubsystem.h"
#include "Kismet/GameplayStatics.h"

void UTechnoMageSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogTemp, Log, TEXT("TechnoMageSubsystem initialized."));
}

void UTechnoMageSubsystem::Deinitialize()
{
	UE_LOG(LogTemp, Log, TEXT("TechnoMageSubsystem deinitialized."));
	Super::Deinitialize();
}

void UTechnoMageSubsystem::ExecuteDialogueEvent(FName EventName)
{
	if (EventName == "StartQuest")
	{
		// Логика для старта квеста
		UE_LOG(LogTemp, Log, TEXT("Started quest!"));
	}
	else if (EventName == "GiveItem")
	{
		// Логика для выдачи предмета
		UE_LOG(LogTemp, Log, TEXT("Gave item to player!"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Unknown event: %s"), *EventName.ToString());
	}
}
