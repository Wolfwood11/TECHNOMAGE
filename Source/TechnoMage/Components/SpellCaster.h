#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SpellCaster.generated.h"

class ABaseSpell;
class AObjectPool;
class UCharacterResourcePool;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TECHNOMAGE_API USpellCaster : public UActorComponent
{
	GENERATED_BODY()

public:
	USpellCaster();

	// Запустить каст следующего заклинания
	UFUNCTION(BlueprintCallable, Category = "Spell Caster")
	void CastNextSpell();

	void SetManaPool(UCharacterResourcePool* manaPoll);
	AActor* FindClosestTarget() const;

protected:
	virtual void BeginPlay() override;
	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Проверить, можно ли запустить каст заклинания
	bool CanCastSpell(ABaseSpell* Spell) const;

	// Выполнить каст заклинания
	void ExecuteSpell(ABaseSpell* Spell);

private:
	// Текущее заклинание
	int32 CurrentSpellIndex;

	// Время до следующего каста
	float CasterCooldownTimer;

	// Ссылка на пул объектов
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spell Caster", meta = (AllowPrivateAccess = "true"))
	AObjectPool* ObjectPool;

	// Ссылка на компонент маны
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spell Caster", meta = (AllowPrivateAccess = "true"))
	UCharacterResourcePool* ManaPool;

	// Очередь заклинаний для каста
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spell Caster", meta = (AllowPrivateAccess = "true"))
	TArray<int32> SpellQueue;

	// Кулдаун между повторами всей очереди
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spell Caster", meta = (AllowPrivateAccess = "true"))
	float CasterCooldown = 0.5f;
	float TargetingRadius = 5000;
	float TargetingAngle = 15.f;
};
