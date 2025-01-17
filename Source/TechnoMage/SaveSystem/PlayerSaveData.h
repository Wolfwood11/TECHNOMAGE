// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerSaveData.generated.h"

// Структура для данных игрока
USTRUCT(BlueprintType)
struct FPlayerSaveData
{
	GENERATED_BODY()

	// Текущая позиция игрока
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player")
	FVector PlayerLocation;

	// Текущий уровень здоровья
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player")
	float Health;

	// Текущий уровень маны
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player")
	float Mana;

	// Количество очков опыта
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player")
	int32 Experience;

	FPlayerSaveData()
		: PlayerLocation(FVector::ZeroVector), Health(100.0f), Mana(100.0f), Experience(0)
	{
	}
};

