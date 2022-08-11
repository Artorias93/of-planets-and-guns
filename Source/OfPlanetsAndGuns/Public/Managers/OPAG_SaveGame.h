// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Player/OPAG_PlayerState.h"
#include "GameFramework/SaveGame.h"
#include "OPAG_SaveGame.generated.h"

/**
 * 
 */
UCLASS()
class OFPLANETSANDGUNS_API UOPAG_SaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UOPAG_SaveGame();

	UPROPERTY(VisibleAnywhere, Category = "Player State")
		FOPAG_PlayerState PlayerState;

	UPROPERTY(VisibleAnywhere, Category = Basic)
		FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
		uint32 UserIndex;
};
