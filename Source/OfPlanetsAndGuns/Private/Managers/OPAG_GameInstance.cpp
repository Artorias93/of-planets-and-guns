// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/OPAG_GameInstance.h"

#include "Managers/OPAG_EventManagerSubsystem.h"
#include "Kismet/GameplayStatics.h"

void UOPAG_GameInstance::BindEvents()
{
	UOPAG_EventManagerSubsystem* EMS = GetSubsystem<UOPAG_EventManagerSubsystem>();
	EMS->StageManagerReady.AddDynamic(this, &UOPAG_GameInstance::OnStageManagerReady);
	EMS->StageLoaded.AddDynamic(this, &UOPAG_GameInstance::OnStageLoaded);

	EMS->RoomCleared.AddDynamic(this, &UOPAG_GameInstance::OnRoomCleared);
	EMS->PauseGame.AddDynamic(this, &UOPAG_GameInstance::OnPauseGame);
	EMS->ResumeGame.AddDynamic(this, &UOPAG_GameInstance::OnResumeGame);
}

void UOPAG_GameInstance::LoadGenerationTable()
{
	FString context;
	//StageGenerationTable->GetAllRows<FOPAG_StageGeneration>(context, StageGens);

	//sort by StageIndex
	//StageGens.Sort();
}

void UOPAG_GameInstance::Init()
{
	Super::Init();

	StageDifficulty = 1.f;

	BindEvents();

	LoadGenerationTable();
}

void UOPAG_GameInstance::OnStageManagerReady()
{


	//GetEventManager()->GenerateStage.Broadcast(*StageGens[0]);
}

void UOPAG_GameInstance::OnStageLoaded()
{
	UE_LOG(LogTemp, Warning, TEXT("QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ"));
}

void UOPAG_GameInstance::OnRoomCleared()
{
	StageDifficulty += 0.2f;
}

void UOPAG_GameInstance::OnPauseGame()
{
	UGameplayStatics::SetGamePaused(GetWorld(), true);
}

void UOPAG_GameInstance::OnResumeGame()
{
	UGameplayStatics::SetGamePaused(GetWorld(), false);
}

UOPAG_EventManagerSubsystem* UOPAG_GameInstance::GetEventManager()
{
	return GetSubsystem<UOPAG_EventManagerSubsystem>();
}

float UOPAG_GameInstance::GetStageDifficulty()
{
	return StageDifficulty;
}
