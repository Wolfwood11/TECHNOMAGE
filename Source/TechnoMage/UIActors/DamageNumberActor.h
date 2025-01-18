#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageNumberActor.generated.h"

UCLASS()
class TECHNOMAGE_API ADamageNumberActor : public AActor
{
	GENERATED_BODY()

public:
	ADamageNumberActor();

	// Инициализация текста урона
	void Initialize(float DamageAmount);

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	// Компонент для отображения текста
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UTextRenderComponent* TextRenderComponent;

	// Скорость подъёма текста
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage", meta = (AllowPrivateAccess = "true"))
	float RiseSpeed = 100.0f;

	// Время жизни текста
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage", meta = (AllowPrivateAccess = "true"))
	float LifeSpan = 1.5f;

	// Оставшееся время жизни
	float RemainingLifeTime;
};
