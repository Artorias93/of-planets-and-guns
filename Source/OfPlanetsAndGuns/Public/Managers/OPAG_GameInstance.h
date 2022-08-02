// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Player/OPAG_PlayerState.h"
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
	EStageType NextStage;
	TArray<FOPAG_StageGeneration*> StageGens;
	TArray<FOPAG_StageGeneration*> BossStages;
	TArray<FOPAG_StageGeneration*> SafeStages;
	int32 CurrentStage;
	uint8 bNextStageIsSafe : 1;

	FOPAG_PlayerState PlayerState;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage Data")
		UDataTable* StageGenerationTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Planet Data")
		TSoftObjectPtr<UWorld> PlanetPersistentLevel;

private:
	void BindEvents();
	void LoadGenerationTable();

	void SplitStageGens();

public:

	virtual void Init() override;

	UFUNCTION(BlueprintCallable)
		void StartStage(const EStageType StageType);

	UFUNCTION(BlueprintCallable)
		void SavePlayerState(FOPAG_PlayerState NewPlayerState);
	UFUNCTION(BlueprintCallable)
		void SendPlayerState();

	UFUNCTION()
		void OnStageManagerReady();
	UFUNCTION()
		void OnStageLoaded();

	UFUNCTION()
		void OnStageCleared(const EStageType NextStageType);

	UFUNCTION()
		void OnRoomEntered();
	UFUNCTION()
		void OnRoomCleared();

	UFUNCTION()
		void OnPauseGame();
	UFUNCTION()
		void OnResumeGame();

	UFUNCTION(BlueprintCallable, Category = "Event Manger")
		UOPAG_EventManagerSubsystem* GetEventManager() const;

	UFUNCTION(BlueprintCallable)
		float GetStageDifficulty();

};
