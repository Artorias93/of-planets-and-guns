// Fill out your copyright notice in the Description page of Project Settings.
#include "Environment/OPAG_DoorBase.h"
#include "Managers/OPAG_GameInstance.h"
#include "Managers/OPAG_SoundManagerSubsystem.h"


// Sets default values
AOPAG_DoorBase::AOPAG_DoorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AOPAG_DoorBase::BeginPlay()
{
	Super::BeginPlay();

	SMS = GetGameInstance<UOPAG_GameInstance>()->GetSoundManager();
}

// Called every frame
void AOPAG_DoorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AOPAG_DoorBase::OpenDoor_Implementation()
{
}

void AOPAG_DoorBase::CloseDoor_Implementation()
{
}

