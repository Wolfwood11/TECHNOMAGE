#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameFramework/Actor.h"
#include "ActorTrackingSubsystem.generated.h"

class AObjectPool;

UCLASS()
class TECHNOMAGE_API UActorTrackingSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	// Зарегистрировать актёра
	void RegisterActor(AObjectPool* Actor);

	// Удалить актёра из регистрации
	void UnregisterActor(AObjectPool* Actor);

	// Получить зарегистрированный актёр
	AObjectPool* GetRegisteredActor() const;

private:
	TWeakObjectPtr<AObjectPool> RegisteredActor;
};
