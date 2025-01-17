#pragma once

#include "CoreMinimal.h"
#include "SpellElement.generated.h"

UENUM(BlueprintType)
enum class ESpellElement : uint8
{
	Normal UMETA(DisplayName = "Normal"), // Независимый элемент
	Fire UMETA(DisplayName = "Fire"),
	Water UMETA(DisplayName = "Water"),
	Earth UMETA(DisplayName = "Earth"),
	Air UMETA(DisplayName = "Air"),
	Chaos UMETA(DisplayName = "Chaos")    // Независимый элемент
};
