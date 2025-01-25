#pragma once

#include "CoreMinimal.h"
#include "TechnoMage/Modificators/ModifierData.h"
#include "TechnoMage/Spels/DamageResult.h"
#include "UObject/Interface.h"
#include "DamageableInterface.generated.h"

// Этот класс не нуждается в реализации методов
UINTERFACE(MinimalAPI, Blueprintable)
class UDamageableInterface : public UInterface
{
	GENERATED_BODY()
};

class TECHNOMAGE_API IDamageableInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Damage Setters")
	void ApplyDamage(const FDamageResult& damageResult);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Damage Getters")
	bool IsAlive() const;
};
