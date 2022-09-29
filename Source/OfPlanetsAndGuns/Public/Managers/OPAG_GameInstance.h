// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OPAG_Settings.h"
#include "Characters/Player/OPAG_PlayerState.h"
#include "Engine/DataTable.h"
#include "Engine/GameInstance.h"
#include "RoomManager/OPAG_RoomManager.h"
#include "StageManager/OPAG_StageGeneration.h"
#include "OPAG_GameInstance.generated.h"

class UOPAG_EventManagerSubsystem;
class UOPAG_SoundManagerSubsystem;
/**
 * 
 */
UCLASS()
class OFPLANETSANDGUNS_API UOPAG_GameInstance : public UGameInstance
{
	GENERATED_BODY()

	float StageDLStart;
	float CurrentDifficulty;
	EStageType NextStage;
	TArray<FOPAG_PlanetData*> Planets;
	TArray<FOPAG_StageGeneration*> StageGens;
	TArray<FOPAG_StageGeneration*> BossStages;
	TArray<FOPAG_StageGeneration*> SafeStages;
	FOPAG_PlanetData* CurrentPlanet;
	int32 CurrentStage;
	int32 CurrentStageRoomCount;
	uint8 bNextStageIsSafe : 1;
	const UWorld* CurrentWorld;

	FOPAG_PlayerState PlayerState;
	FOPAG_Settings Settings;

	const char* PlayerSaveSlotName = "PlayerSaveSlot";

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Planet Data")
		UDataTable* PlanetTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Main Menu")
		TSoftObjectPtr<UWorld> MainMenuLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FMOD | Audio Libraries")
		UDataTable* WeaponsAudioLibrary;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FMOD | Audio Libraries")
		UDataTable* EnviromentAudioLibrary;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FMOD | Audio Libraries")
		UDataTable* AbilitiesAudioLibrary;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FMOD | Audio Libraries")
		UDataTable* CharacterAudioLibrary;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FMOD | Audio Libraries")
		UDataTable* UIAudioLibrary;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FMOD | Audio Libraries")
		UDataTable* SoundtracksAudioLibrary;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FMOD | Buses")
		class UFMODBus* AudioMasterBus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FMOD | Buses")
		class UFMODBus* AudioMusicBus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FMOD | Buses")
		class UFMODBus* AudioSFXBus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FMOD | Buses")
		class UFMODBus* AudioAmbientBus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FMOD | Buses")
		class UFMODBus* AudioSystemBus;

private:
	void BindEvents();
	void LoadPlanetTable();
	void LoadGenerationTable();

	void SplitStageGens();

	void LoadSaveGameState();
	void SaveGameState();

public:

	virtual void Init() override;

	UFUNCTION(BlueprintCallable)
		void LoadMainMenu();

	UFUNCTION(BlueprintCallable)
		void LoadPlanet(int PlanetIndex);

	UFUNCTION(BlueprintCallable)
		void StartStage(const EStageType StageType);

	UFUNCTION(BlueprintCallable)
		void SavePlayerState(FOPAG_PlayerState NewPlayerState);
	UFUNCTION(BlueprintCallable)
		void SendPlayerState();
	UFUNCTION(BlueprintCallable)
		void SaveSettings(FOPAG_Settings NewSettings);
	UFUNCTION(BlueprintCallable)
		void SendSettings();

	UFUNCTION()
		void OnStageManagerInitialized(const UWorld* const World);
	UFUNCTION()
		void OnStageManagerReady();
	UFUNCTION()
		void OnStageLoaded(int32 RoomCount);

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
	UFUNCTION()
		void OnPlayerDeath();
	UFUNCTION()
		void OnQualitySettingsChanged(int32 Value);

	UFUNCTION()
		void SetAudioLibraries();

	UFUNCTION(BlueprintCallable, Category = "Event Manger")
		UOPAG_EventManagerSubsystem* GetEventManager() const;

	UFUNCTION(BlueprintCallable, Category = "Sound Manager")
		UOPAG_SoundManagerSubsystem* GetSoundManager() const;

	UFUNCTION(BlueprintCallable)
		float GetStageDifficulty();

};
