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

	// Получить объект из пула по ключу
	class ABasePoolableActor* GetObject(int32 Key);
	class ABasePoolableActor* GetObject(const TSubclassOf<class ABasePoolableActor>& Type);

	// Вернуть объект в пул
	void ReturnObject(ABasePoolableActor* Object);


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pool Keeper")
	TArray<TObjectPtr<ABasePoolableActor>> _actors;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// Конфигурация пула: ключ (int32) и класс для спавна
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pool Configuration")
	TMap<int32, TSubclassOf<class ABasePoolableActor>> PoolConfig;

	// Базовый размер пула для каждого типа объектов
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pool Configuration")
	int32 InitialPoolSize;
private:
	// Мапа для хранения объектов пула (по ключу)
	TMap<int32, TArray<ABasePoolableActor*>> ObjectPools;

	// Инициализация пула
	void InitializePool();

	// Создать новый объект и добавить его в пул
	ABasePoolableActor* CreateNewObject(int32 Key);
};
