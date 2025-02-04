#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ActionLockInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UActionLockInterface : public UInterface
{
	GENERATED_BODY()
};

class IActionLockInterface
{
	GENERATED_BODY()

public:
	// Заблокировать действие
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Damage Setters")
	bool Lock(TSubclassOf<UActorComponent> ComponentClass, bool DisableMovement);

	// Разблокировать действие
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Damage Setters")
	bool UnLock(TSubclassOf<UActorComponent> ComponentClass);

	// Проверить, заблокировано ли действие
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Damage Setters")
	bool IsLocked() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Damage Setters")
	bool IsLockedByMe(TSubclassOf<UActorComponent> ComponentClass) const;
};
