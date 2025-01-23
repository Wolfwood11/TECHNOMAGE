#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TechnoMage/Enums/Enums.h"
#include "TechnoMage/Interfaces/SaveableInterface.h"
#include "LevelingComponent.generated.h"

class UCharacterParamData;
struct FLevelData;
struct FStatParams;
class UStatsData;

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

	UPROPERTY()
	TMap<ECharacterStatType, int32> AllocatedStats;
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

	UFUNCTION(BlueprintCallable, Category = "Leveling")
	int32 GetAvailableStatPoints() const;

	UFUNCTION(BlueprintCallable, Category = "Leveling")
	int32 GetStatUpgradeCost(ECharacterStatType StatType) const;


	// Добавить очки в стат
	UFUNCTION(BlueprintCallable, Category = "Leveling|Stats")
	bool AllocateStatPoint(ECharacterStatType StatType);

	UFUNCTION(BlueprintCallable, Category = "Leveling|Stats")
	int32 GetStatValue(ECharacterStatType StatType) const;

	// Получение множителя для стата
	UFUNCTION(BlueprintCallable, Category = "Leveling|Stats")
	float GetStatMultiplier(ECharacterStatType StatType) const;

	// Установить данные статов
	UFUNCTION(BlueprintCallable, Category = "Leveling")
	void SetStatsData(UStatsData* NewStatsData);

	UPROPERTY(BlueprintAssignable, Category = "Leveling")
	FOnLevelUp OnLevelUp;

	UFUNCTION(BlueprintCallable, Category = "Leveling|Params")
	float GetCharacterParam(ECharacterParamType ParamType) const;

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

	// Назначенные очки в статы
	TMap<ECharacterStatType, int32> AllocatedStats;

	// Таблица данных уровней
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Leveling", meta = (AllowPrivateAccess = "true"))
	UDataTable* LevelDataTable;

	// Данные параметров персонажа
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Leveling", meta = (AllowPrivateAccess = "true"))
	UCharacterParamData* CharacterParamData;

	// Данные множителей статов
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Leveling", meta = (AllowPrivateAccess = "true"))
	UStatsData* StatsData;

	// Получить данные для текущего уровня
	const FLevelData* GetLevelData(int32 Level) const;
};
