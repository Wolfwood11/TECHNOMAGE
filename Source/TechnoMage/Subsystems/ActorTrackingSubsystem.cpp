#include "ActorTrackingSubsystem.h"

#include "TechnoMage/ObjectPool.h"

void UActorTrackingSubsystem::RegisterActor(TObjectPtr<AObjectPool> Actor)
{
	if (!Actor)
	{
		UE_LOG(LogTemp, Warning, TEXT("RegisterActor: Null actor cannot be registered."));
		return;
	}

	RegisteredActor = Actor;
	UE_LOG(LogTemp, Log, TEXT("Actor %s registered."), *Actor->GetName());
}

void UActorTrackingSubsystem::UnregisterActor(TObjectPtr<AObjectPool> Actor)
{
	if (RegisteredActor == Actor)
	{
		RegisteredActor = nullptr;
		UE_LOG(LogTemp, Log, TEXT("Actor %s unregistered."), *Actor->GetName());
	}
}

TObjectPtr<AObjectPool>  UActorTrackingSubsystem::GetRegisteredActor() const
{
	return RegisteredActor.Get();
}
