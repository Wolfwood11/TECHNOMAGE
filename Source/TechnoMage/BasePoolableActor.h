#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasePoolableActor.generated.h"

UCLASS()
class TECHNOMAGE_API ABasePoolableActor : public AActor
{
	GENERATED_BODY()

public:
	ABasePoolableActor();

	bool IsPooled() const;

	virtual void GetFromPool();
	// Возвращает объект в пул
	virtual void ReturnToPool();

	virtual void ActivateFromPoll(const FTransform& transform, AActor* InstigatorActor);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pool Configuration", meta = (AllowPrivateAccess = "true"))
	float ActiveLifeTime = 1.5f;

	UFUNCTION(BlueprintCallable, Category = "Pool")
	AActor* GetInstigatorActor() const;

	void Tick(float DeltaSeconds) override;

protected:
	// Ссылка на актёра, инициировавшего активацию
	TWeakObjectPtr<AActor> InstigatorActor;

	float CurrentActiveLifeTime = 0.f;

	bool isPooled = true;

	virtual void UpdateComponentsState(bool value);
};
