#pragma once

#include "CoreMinimal.h"
#include "TechnoMage/Modificators/ModifierData.h"
#include "TechnoMage/Spels/DamageResult.h"
#include "UObject/Interface.h"
#include "DamageableInterface.generated.h"

// Этот класс не нуждается в реализации методов
UINTERFACE(MinimalAPI)
class UDamageableInterface : public UInterface
{
	GENERATED_BODY()
};

class TECHNOMAGE_API IDamageableInterface
{
	GENERATED_BODY()

public:
	virtual void ApplyDamage(const FDamageResult& damageResult) = 0;
	virtual bool IsAlive() const = 0;
};
