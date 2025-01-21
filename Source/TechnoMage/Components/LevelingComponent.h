#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TechnoMage/Interfaces/SaveableInterface.h"
#include "LevelingComponent.generated.h"

struct FLevelData;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevelUp, int32, NewLevel);

USTRUCT(BlueprintType)
struct FLevelingSaveData
{
	GENERATED_BODY()

	UPROPERTY()
	int32 CurrentLevel;

	UPROPERTY()
	int32 CurrentExperience;

	UPROPERTY()
	int32 SkillPoints;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TECHNOMAGE_API ULevelingComponent : public UActorComponent, public ISaveableInterface
{
	GENERATED_BODY()

public:
	ULevelingComponent();

	virtual void SaveData_Implementation(TArray<uint8>& OutData) override;
	virtual void LoadData_Implementation(const TArray<uint8>& InData) override;

	// Добавление опыта
	UFUNCTION(BlueprintCallable, Category = "Leveling")
	void AddExperience(int32 Amount);

	// Получение текущего уровня
	UFUNCTION(BlueprintCallable, Category = "Leveling")
	int32 GetLevel() const;

	// Получение текущего опыта
	UFUNCTION(BlueprintCallable, Category = "Leveling")
	int32 GetExperience() const;

	// Получение опыта до следующего уровня
	UFUNCTION(BlueprintCallable, Category = "Leveling")
	int32 GetExperienceToNextLevel() const;

	// Сохранение данных
	FLevelingSaveData SaveData() const;

	// Загрузка данных
	void LoadData(const FLevelingSaveData& Data);

	// Установить таблицу данных уровня
	UFUNCTION(BlueprintCallable, Category = "Leveling")
	void SetLevelDataTable(UDataTable* NewLevelDataTable);

	UPROPERTY(BlueprintAssignable, Category = "Leveling")
	FOnLevelUp OnLevelUp;

protected:
	virtual void BeginPlay() override;

	// Проверка на повышение уровня
	void CheckForLevelUp();

private:
	// Текущий уровень
	int32 CurrentLevel;

	// Текущий опыт
	int32 CurrentExperience;

	// Количество скилл-поинтов
	int32 SkillPoints;

	// Таблица данных уровней
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Leveling", meta = (AllowPrivateAccess = "true"))
	UDataTable* LevelDataTable;

	// Получить данные для текущего уровня
	const FLevelData* GetLevelData(int32 Level) const;
};
