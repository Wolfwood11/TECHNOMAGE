#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObjectPool.generated.h"

UCLASS()
class TECHNOMAGE_API AObjectPool : public AActor
{
	GENERATED_BODY()

public:
	AObjectPool();

	// Получить объект из пула по типу
	class ABasePoolableActor* GetObject(const TSubclassOf<class ABasePoolableActor>& Type);

	// Вернуть объект в пул
	void ReturnObject(ABasePoolableActor* Object);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// Конфигурация пула: тип объекта и количество предзагруженных
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pool Configuration")
	TMap<TSubclassOf<class ABasePoolableActor>, int32> PoolConfig;

private:
	// Мапа для хранения объектов пула
	TMap<TSubclassOf<class ABasePoolableActor>, TArray<ABasePoolableActor*>> ObjectPools;

	// Инициализация пула
	void InitializePool();

	// Создать новый объект и добавить его в пул
	ABasePoolableActor* CreateNewObject(const TSubclassOf<ABasePoolableActor>& Type);
};
