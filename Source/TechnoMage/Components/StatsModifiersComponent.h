#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Modificators/ModifierStateData.h"
#include "StatsModifiersComponent.generated.h"


class ABaseSpell;
struct FDamageResult;
// Компонент для управления модификаторами
UCLASS(BlueprintType, Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TECHNOMAGE_API UStatsModifiersComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UStatsModifiersComponent();
	static bool AreElementsOpposite(ESpellElement Element1, ESpellElement Element2);
	static bool AreElementsBoosts(ESpellElement Element1, ESpellElement Element2);

	UFUNCTION(BlueprintCallable, Category = "Modifiers")
	TArray<UModifierData*> GetModifiers(EModifierType Type) const;

	UFUNCTION(BlueprintCallable, Category = "Modifiers")
	void  RemoveOpposite(ESpellElement element);

	UFUNCTION(BlueprintCallable, Category = "Modifiers")
	static void ProcessModifiers(const TArray<UModifierData*>& Modifiers, float& inValue);

	// Добавить модификатор
	UFUNCTION(BlueprintCallable, Category = "Modifiers")
	void ApplyModifier(const FDamageResult& DamageResult);
	void RemoveModifier(const UModifierData* Modifier);
	void HandleSubSpells(const FDamageResult& DamageResult, FModifierStateData& ActiveElementalModifier);
	void SpawnSubSpell(const TSubclassOf<ABaseSpell>& SubSpell) const;

	static bool IsElementalModifier(ESpellElement Element);

	// Очистить все модификаторы
	UFUNCTION(BlueprintCallable, Category = "Modifiers")
	void ClearAllModifiers();

	// Очистить модификаторы определённого типа
	UFUNCTION(BlueprintCallable, Category = "Modifiers")
	void ClearModifiersOfType(EModifierType Type);

	// Сохранить активные модификаторы
	UFUNCTION(BlueprintCallable, Category = "Modifiers")
	void SaveModifiers();

	// Загрузить активные модификаторы
	UFUNCTION(BlueprintCallable, Category = "Modifiers")
	void LoadModifiers();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	// Список активных модификаторов
	UPROPERTY()
	TArray<FModifierStateData> ActiveModifiers;

	UPROPERTY()
	float TimeSinceLastSave;

	UPROPERTY()
	float SaveInterval = 5.0f;

	// Деактивация партикл-эффектов
	void DeactivateEffects(FModifierStateData& ModifierState);
	void ActivateEffects(FModifierStateData& ModifierState);

	// Реактивация партикл-эффектов при загрузке
	void ReactivateEffects(FModifierStateData& ModifierState);
};
