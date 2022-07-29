// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/OPAG_GameInstance.h"

#include "Managers/OPAG_EventManagerSubsystem.h"
#include "Kismet/GameplayStatics.h"

void UOPAG_GameInstance::BindEvents()
{
	UOPAG_EventManagerSubsystem* EMS = GetSubsystem<UOPAG_EventManagerSubsystem>();
	EMS->StageManagerReady.AddDynamic(this, &UOPAG_GameInstance::OnStageManagerReady);
	EMS->StageLoaded.AddDynamic(this, &UOPAG_GameInstance::OnStageLoaded);
	EMS->StageCleared.AddDynamic(this, &UOPAG_GameInstance::OnStageCleared);

	EMS->RoomEntered.AddDynamic(this, &UOPAG_GameInstance::OnRoomEntered);
	EMS->RoomCleared.AddDynamic(this, &UOPAG_GameInstance::OnRoomCleared);
	EMS->PauseGame.AddDynamic(this, &UOPAG_GameInstance::OnPauseGame);
	EMS->ResumeGame.AddDynamic(this, &UOPAG_GameInstance::OnResumeGame);
}

void UOPAG_GameInstance::LoadGenerationTable()
{
	FString context;
	StageGenerationTable->GetAllRows<FOPAG_StageGeneration>(context, StageGens);

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

void UOPAG_GameInstance::Init()
{
	Super::Init();

	StageDifficulty = 1.f;
	CurrentStage = 0;
	bNextStageIsSafe = false;

	BindEvents();

	LoadGenerationTable();
}

void UOPAG_GameInstance::StartStage(const EStageType StageType)
{
	UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), PlanetPersistentLevel);
}

void UOPAG_GameInstance::SavePlayerState()
{
	//TODO: add player state
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

void UOPAG_GameInstance::OnStageLoaded()
{
	UE_LOG(LogTemp, Warning, TEXT("QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ"));
}

void UOPAG_GameInstance::OnStageCleared(const EStageType NextStageType)
{
	switch (NextStageType) { case EStageType::Stage: 
		CurrentStage++;
		break;
	case EStageType::SafeRoom:
		bNextStageIsSafe = true;
		break;
	case EStageType::BossRoom: break;
	default: ;
	}


	StartStage(NextStageType);
}

void UOPAG_GameInstance::OnRoomEntered()
{
	StageDifficulty += 0.2f;
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

float UOPAG_GameInstance::GetStageDifficulty()
{
	return StageDifficulty;
}
