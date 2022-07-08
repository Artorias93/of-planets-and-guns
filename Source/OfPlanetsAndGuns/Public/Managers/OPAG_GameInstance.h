// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/GameInstance.h"
#include "StageManager/OPAG_StageGeneration.h"
#include "OPAG_GameInstance.generated.h"

class UOPAG_EventManagerSubsystem;
/**
 * 
 */
UCLASS()
class OFPLANETSANDGUNS_API UOPAG_GameInstance : public UGameInstance
{
	GENERATED_BODY()

	float StageDifficulty;
	TArray<FOPAG_StageGeneration*> StageGens;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage Data")
		UDataTable* StageGenerationTable;

private:
	void BindEvents();
	void LoadGenerationTable();

public:

	virtual void Init() override;

	UFUNCTION()
		void OnStageManagerReady();
	UFUNCTION()
		void OnStageLoaded();

	UFUNCTION()
		void OnRoomEntered();
	UFUNCTION()
		void OnRoomCleared();

	UFUNCTION()
		void OnPauseGame();
	UFUNCTION()
		void OnResumeGame();

	UFUNCTION(BlueprintCallable, Category = "Event Manger")
		UOPAG_EventManagerSubsystem* GetEventManager();

	UFUNCTION(BlueprintCallable)
		float GetStageDifficulty();

};
