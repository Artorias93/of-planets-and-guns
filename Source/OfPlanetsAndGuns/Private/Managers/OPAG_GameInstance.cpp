// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/OPAG_GameInstance.h"

#include "Managers/OPAG_EventManagerSubsystem.h"
#include "Managers/OPAG_SoundManagerSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/OPAG_SaveGame.h"
#include "Util/OPAG_UtilityFunctionLibrary.h"

void UOPAG_GameInstance::BindEvents()
{
	UOPAG_EventManagerSubsystem* EMS = GetSubsystem<UOPAG_EventManagerSubsystem>();
	EMS->StageManagerInit.AddDynamic(this, &UOPAG_GameInstance::OnStageManagerInitialized);
	EMS->StageManagerReady.AddDynamic(this, &UOPAG_GameInstance::OnStageManagerReady);
	EMS->StageLoaded.AddDynamic(this, &UOPAG_GameInstance::OnStageLoaded);
	EMS->StageCleared.AddDynamic(this, &UOPAG_GameInstance::OnStageCleared);

	EMS->RoomEntered.AddDynamic(this, &UOPAG_GameInstance::OnRoomEntered);
	EMS->RoomCleared.AddDynamic(this, &UOPAG_GameInstance::OnRoomCleared);
	EMS->PauseGame.AddDynamic(this, &UOPAG_GameInstance::OnPauseGame);
	EMS->ResumeGame.AddDynamic(this, &UOPAG_GameInstance::OnResumeGame);

	EMS->LoadMainMenu.AddDynamic(this, &UOPAG_GameInstance::LoadMainMenu);
	EMS->StartPlanet.AddDynamic(this, &UOPAG_GameInstance::LoadPlanet);


	EMS->RequestPlayerState.AddDynamic(this, &UOPAG_GameInstance::SendPlayerState);
	EMS->SavePlayerState.AddDynamic(this, &UOPAG_GameInstance::SavePlayerState);
}

void UOPAG_GameInstance::LoadPlanetTable()
{
	FString context;
	PlanetTable->GetAllRows<FOPAG_PlanetData>(context, Planets);
}

void UOPAG_GameInstance::LoadGenerationTable()
{
	FString context;
	StageGens.Empty();
	SafeStages.Empty();
	BossStages.Empty();
	CurrentPlanet->StageGenerationTable->GetAllRows<FOPAG_StageGeneration>(context, StageGens);

	SplitStageGens();

	//sort by StageIndex
	StageGens.Sort();
}

void UOPAG_GameInstance::SplitStageGens()
{
	for (FOPAG_StageGeneration* StageGen : StageGens)
	{
		switch (StageGen->StageType) {
			case EStageType::Stage: break;
			case EStageType::SafeRoom:
				SafeStages.Add(StageGen);
				break;
			case EStageType::BossRoom:
				BossStages.Add(StageGen);
				break;
			default: ;
		}
	}

	for (FOPAG_StageGeneration* StageGen : SafeStages)
	{
		StageGens.RemoveSingle(StageGen);
	}

	for (FOPAG_StageGeneration* StageGen : BossStages)
	{
		StageGens.RemoveSingle(StageGen);
	}
}

void UOPAG_GameInstance::LoadSaveGameState()
{
	// Retrieve and cast the USaveGame object to UOPAG_SaveGame.
	if (UOPAG_SaveGame* LoadedGame = Cast<UOPAG_SaveGame>(UGameplayStatics::LoadGameFromSlot(
		PlayerSaveSlotName, 0)))
	{
		PlayerState = LoadedGame->PlayerState;
		PlayerState.LoadType = PlayerStateLoadType::SAVE_FILE;

		UOPAG_UtilityFunctionLibrary::ScreenLogWithSeverityColor(GetWorld(),
			"Player Data Loaded", EMessageSeverity::Info);
	}
}

void UOPAG_GameInstance::SaveGameState()
{
	if (UOPAG_SaveGame* SaveGameInstance = Cast<UOPAG_SaveGame>(
		UGameplayStatics::CreateSaveGameObject(UOPAG_SaveGame::StaticClass())))
	{

		// Set data on the savegame object.
		SaveGameInstance->PlayerState = PlayerState;

		// Start async save process.
		UGameplayStatics::AsyncSaveGameToSlot(SaveGameInstance, 
			PlayerSaveSlotName, 0,
			FAsyncSaveGameToSlotDelegate::CreateLambda(
				[this](const FString& SlotName, const int32 SlotIndex, bool Success)
				{
					UOPAG_UtilityFunctionLibrary::ScreenLogWithSeverityColor(GetWorld(),
					Success ? "Player Data Saved" : "Player Data Failed To Save",
						EMessageSeverity::Info);
				})
		);
	}
}

void UOPAG_GameInstance::Init()
{
	Super::Init();

	CurrentDifficulty = 1.f;
	CurrentStage = 0;
	bNextStageIsSafe = false;

	BindEvents();
	SetAudioLibraries();

	LoadPlanetTable();
	CurrentPlanet = Planets[0];
	LoadGenerationTable();
}

void UOPAG_GameInstance::LoadMainMenu()
{
	UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), MainMenuLevel);
}

void UOPAG_GameInstance::LoadPlanet(int PlanetIndex)
{
	CurrentPlanet = Planets[PlanetIndex];
	StartStage(EStageType::Stage);
}

void UOPAG_GameInstance::StartStage(const EStageType StageType)
{
	UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), CurrentPlanet->PlanetPersistentLevel);
}

void UOPAG_GameInstance::SavePlayerState(FOPAG_PlayerState NewPlayerState)
{
	PlayerState = NewPlayerState;
	SaveGameState();
	if (PlayerState.Health > 0)
		StartStage(NextStage);
}

void UOPAG_GameInstance::SendPlayerState()
{
	const auto World = GetWorld();
	if (!CurrentWorld || !World || World->GetFullName() != CurrentWorld->GetFullName())
	{
		GetEventManager()->PlacePlayerAtStart.Broadcast(
			GetWorld()->GetFirstPlayerController()->GetPawnOrSpectator()->GetActorTransform());
		// pretty bad hardcoded 2 here.. but we don't want 1 cause then it's gonna divide by 0
		// (cause it removes the starting room)
		GetEventManager()->StageLoaded.Broadcast(2);
		return;
	}

	if (PlayerState.LoadType == PlayerStateLoadType::NOT_LOADED)
		LoadSaveGameState();

	if (PlayerState.LoadType != PlayerStateLoadType::NOT_LOADED)
		GetEventManager()->ApplyPlayerState.Broadcast(PlayerState);
}

void UOPAG_GameInstance::OnStageManagerInitialized(const UWorld* const World)
{
	CurrentWorld = World;
}

void UOPAG_GameInstance::OnStageManagerReady()
{

	if (bNextStageIsSafe)
	{
		bNextStageIsSafe = false;
		GetEventManager()->GenerateStage.Broadcast(*SafeStages[0]);
	}
	else if (StageGens.Last()->StageIndex < CurrentStage)
	{
		GetEventManager()->GenerateStage.Broadcast(*BossStages[0]);
	}
	else
	{
		GetEventManager()->GenerateStage.Broadcast(*StageGens[CurrentStage]);
	}
}

void UOPAG_GameInstance::OnStageLoaded(int32 RoomCount)
{
	UE_LOG(LogTemp, Warning, TEXT("Stage Finished Loading"));
	//ignore the starting room
	CurrentStageRoomCount = RoomCount-1;

	StageDLStart = CurrentPlanet->PlanetDLStart + CurrentPlanet->SSIncrement * CurrentStage;
	CurrentDifficulty = StageDLStart;
	UE_LOG(LogTemp, Warning, TEXT("New Difficulty: %f"), CurrentDifficulty);

}

void UOPAG_GameInstance::OnStageCleared(const EStageType NextStageType)
{
	NextStage = NextStageType;
	switch (NextStageType) { case EStageType::Stage: 
		CurrentStage++;
		break;
	case EStageType::SafeRoom:
		bNextStageIsSafe = true;
		break;
	case EStageType::BossRoom: break;
	default: ;
	}

	GetEventManager()->RequestSavePlayerState.Broadcast();
}

void UOPAG_GameInstance::OnRoomEntered()
{
	CurrentDifficulty += (CurrentPlanet->PlanetDLTarget - StageDLStart) / CurrentStageRoomCount;
	if (CurrentDifficulty > CurrentPlanet->PlanetDLTarget)
		CurrentDifficulty = CurrentPlanet->PlanetDLTarget;
	UE_LOG(LogTemp, Warning, TEXT("New Difficulty: %f"), CurrentDifficulty);
	GetEventManager()->StageDifficultyUpdated.Broadcast();
}

void UOPAG_GameInstance::OnRoomCleared()
{
}

void UOPAG_GameInstance::OnPauseGame()
{
	UGameplayStatics::SetGamePaused(GetWorld(), true);
}

void UOPAG_GameInstance::OnResumeGame()
{
	UGameplayStatics::SetGamePaused(GetWorld(), false);
}

UOPAG_EventManagerSubsystem* UOPAG_GameInstance::GetEventManager() const
{
	return GetSubsystem<UOPAG_EventManagerSubsystem>();
}

UOPAG_SoundManagerSubsystem* UOPAG_GameInstance::GetSoundManager() const
{
	return GetSubsystem<UOPAG_SoundManagerSubsystem>();
}

float UOPAG_GameInstance::GetStageDifficulty()
{
	return CurrentDifficulty;
}

void UOPAG_GameInstance::SetAudioLibraries()
{
	UOPAG_SoundManagerSubsystem* SMS = GetSubsystem<UOPAG_SoundManagerSubsystem>();
	SMS->WeaponsAudioLibrary = WeaponsAudioLibrary;
	SMS->AbilitiesAudioLibrary = AbilitiesAudioLibrary;
	SMS->EnviromentAudioLibrary = EnviromentAudioLibrary;
	SMS->CharacterAudioLibrary = CharacterAudioLibrary;
	SMS->UIAudioLibrary = UIAudioLibrary;
}
