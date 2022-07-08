// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/StageManager/OPAG_StageManager.h"

#include "Managers/OPAG_GameInstance.h"
#include "Managers/OPAG_EventManagerSubsystem.h"

// Sets default values
AOPAG_StageManager::AOPAG_StageManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AOPAG_StageManager::StartStageGeneration(FOPAG_StageGeneration GenData)
{
	FinishedLoadingStage();
}

void AOPAG_StageManager::FinishedLoadingStage()
{
	UOPAG_GameInstance* GI = Cast<UOPAG_GameInstance>(GetGameInstance());
	if (GI)
	{
		GI->GetEventManager()->StageLoaded.Broadcast();
	}
}

// Called when the game starts or when spawned
void AOPAG_StageManager::BeginPlay()
{
	Super::BeginPlay();
	UOPAG_GameInstance* GI = Cast<UOPAG_GameInstance>(GetGameInstance());
	if (GI)
	{
		GI->GetEventManager()->GenerateStage.AddDynamic(this, &AOPAG_StageManager::StartStageGeneration);
		GI->GetEventManager()->StageManagerReady.Broadcast();
	}
}

// Called every frame
void AOPAG_StageManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}