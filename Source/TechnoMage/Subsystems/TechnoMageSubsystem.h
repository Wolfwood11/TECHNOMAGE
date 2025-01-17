#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "TechnoMageSubsystem.generated.h"

// Подсистема для работы с глобальными функциями и состоянием игры
UCLASS(Blueprintable)
class TECHNOMAGE_API UTechnoMageSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// Инициализация подсистемы
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	// Деинициализация подсистемы
	virtual void Deinitialize() override;

	// Выполнить событие диалога
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void ExecuteDialogueEvent(FName EventName);
};
