// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SaveableInterface.generated.h"


UINTERFACE(MinimalAPI, Blueprintable)
class USaveableInterface : public UInterface
{
	GENERATED_BODY()
};

class ISaveableInterface
{
	GENERATED_BODY()

public:
	// Сохранение данных актора
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Saveable")
	void SaveData(TArray<uint8>& OutData) const;

	// Загрузка данных актора
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Saveable")
	void LoadData(const TArray<uint8>& InData);
};
