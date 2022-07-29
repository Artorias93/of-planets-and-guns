// Fill out your copyright notice in the Description page of Project Settings.


#include "Rooms/OPAG_Portal.h"

#include "Managers/OPAG_EventManagerSubsystem.h"
#include "Managers/OPAG_GameInstance.h"
#include "Util/OPAG_UtilityFunctionLibrary.h"

// Sets default values
AOPAG_Portal::AOPAG_Portal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

AOPAG_Portal* AOPAG_Portal::SpawnPortal(UWorld* World, const TSubclassOf<AOPAG_Portal> Class, const EStageType Type,
	const FVector& Location, const FRotator& Rotation)
{
	AOPAG_Portal* Portal =  World->SpawnActor<AOPAG_Portal>(Class, Location, Rotation);
	Portal->PortalType = Type;
	return Portal;
}

void AOPAG_Portal::UnlockPortal()
{
	bIsOpen = true;
}

// Called when the game starts or when spawned
void AOPAG_Portal::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOPAG_Portal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AOPAG_Portal::Interact_Implementation()
{
	if (!bIsOpen) return;

	if (PortalType == EStageType::BossRoom)
		UOPAG_UtilityFunctionLibrary::ScreenLogWithSeverityColor(this,
			"BOSS ROOM CALLED DIRECTLY FROM PORTAL",
			EMessageSeverity::Error);

	const auto GI = GetGameInstance<UOPAG_GameInstance>();
	if (!GI) return;

	GI->GetEventManager()->StageCleared.Broadcast(PortalType);
}

bool AOPAG_Portal::CanInteract_Implementation()
{
	return bIsOpen;
}

