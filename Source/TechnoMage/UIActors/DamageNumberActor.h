#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "..//BasePoolableActor.h"
#include "DamageNumberActor.generated.h"

UCLASS()
class TECHNOMAGE_API ADamageNumberActor : public ABasePoolableActor
{
	GENERATED_BODY()

public:
	ADamageNumberActor();

	// Инициализация текста урона
	void Initialize(float DamageAmount, const FTransform& transform, AActor* NewInstigatorActor, bool bIsCritical);

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	FVector Velocity; // Начальная скорость
	FVector Acceleration; // Ускорение для параболической траектории

	float ScaleFactor; // Текущий масштаб
	float TargetScale; // Целевой масштаб
	float ScaleSpeed; // Скорость изменения масштаба

	float HoverTime; // Время зависания в конечной точке
	float HoverTimer; // Таймер зависания

	bool bIsHovering; // Флаг зависания

	FColor CriticalColor = FColor::Yellow; // Цвет для критического удара
	FColor NormalColor = FColor::Red;     // Цвет для обычного удара

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
