// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/RoomManager/OPAG_RoomManager.h"

#include "EngineUtils.h"
#include "Engine/LevelStreaming.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Managers/OPAG_EventManagerSubsystem.h"
#include "Managers/OPAG_GameInstance.h"
#include "Managers/RoomManager/OPAG_DoorManagerComponent.h"
#include "Managers/RoomManager/OPAG_EnemyManagerComponent.h"
#include "Managers/RoomManager/OPAG_RoomOffset.h"
#include "Rooms/OPAG_Portal.h"
#include "Rooms/OPAG_RoomAttachPoint.h"
#include "Rooms/OPAG_RoomStreamingVolume.h"
#include "Interactables/OPAG_LootBox.h"
#include "Managers/RoomManager/OPAG_LootManagerComponent.h"


// Sets default values
AOPAG_RoomManager::AOPAG_RoomManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	DoorManager = CreateDefaultSubobject<UOPAG_DoorManagerComponent>(TEXT("Door Manager"));
	EnemyManager = CreateDefaultSubobject<UOPAG_EnemyManagerComponent>(TEXT("Enemy Manager"));
	LootManager = CreateDefaultSubobject<UOPAG_LootManagerComponent>(TEXT("Loot Manager"));
}


// Called when the game starts or when spawned
void AOPAG_RoomManager::BeginPlay()
{
	Super::BeginPlay();
	bHasBroadcastedEntry = false;
	bIsAwaitingDifficultyUpdate = false;
}

void AOPAG_RoomManager::PreInitializeComponents()
{
	Super::PreInitializeComponents();
	const auto World = AActor::GetWorld();
	if (World != nullptr)
	{
		const auto GameState = World->GetGameState();;
		if (GameState != nullptr && GameState->HasBegunPlay())
		{
			const UOPAG_GameInstance* GI = Cast<UOPAG_GameInstance>(GetGameInstance());
			if (GI)
			{
				GI->GetEventManager()->RoomSpawned.Broadcast(this);
				GI->GetEventManager()->StageDifficultyUpdated.AddDynamic(this, &AOPAG_RoomManager::OnStageDifficultyUpdated);
				return;
			}
		}
	}
}

// Called every frame
void AOPAG_RoomManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AOPAG_RoomManager::UpdateData()
{
	const auto World = GetWorld();
	if (World != nullptr)
	{
		Level = World;
		const auto Name = World->GetName();
		if (GeometryLevel == nullptr)
		{
			for (const ULevelStreaming* StreamingLevel : World->GetStreamingLevels())
			{
				if (StreamingLevel->GetWorldAsset().GetAssetName().Contains(Name))
				{
					GeometryLevel = StreamingLevel->GetWorldAsset();
					break;
				}
			}
		}

		for (TActorIterator<AOPAG_RoomStreamingVolume> It(World); It; ++It)
		{
			Volume = *It;
			break;
		}

		for (TActorIterator<AOPAG_RoomAttachPoint> It(World); It; ++It)
		{
			It->UpdateAttachPointData(this);
		}

		for (TActorIterator<AOPAG_LootBox> It(World); It; ++It)
		{
			LootManager->LootBox = *It;
		}

		if (DataTable != nullptr)
		{
			FOPAG_RoomOffset Row;
			Row.Room = Level;
			Row.Weight = Weight;
			Row.ExitCount = Exits.Num();
			Row.ExitSides = ExitSides;
			Row.bIsStartingRoom = bIsStartingRoom;
			Row.Offset = Entrance != nullptr ? Entrance->RoomOffset : FTransform::Identity;

			DataTable->AddRow(FName(Name), Row);

			UKismetSystemLibrary::PrintString(this, 
				TEXT("Successfully updated Data Table"), 
				true, true, FColor::Green, 3.f);
		}
		else
		{
			UKismetSystemLibrary::PrintString(this,
				TEXT("Failed to update Data Table. Data Table not assigned"),
				true, true, FColor::Red, 3.f);
		}
	}
}

void AOPAG_RoomManager::CloseExits(bool bIsLastRoom, bool bHasSafeRoom) const
{
	for (const AOPAG_RoomAttachPoint* Exit : Exits)
	{
		if (Exit->bIsOpen) continue;

		if (bIsLastRoom || bHasSafeRoom)
		{
			DoorManager->SetExitAsSpecial(bIsLastRoom, bHasSafeRoom,
				Exit->GetActorLocation(), Exit->GetActorRotation());
			bIsLastRoom = false;
			bHasSafeRoom = false;
		}
		else DoorManager->BlockExit(Exit->GetActorLocation(), Exit->GetActorRotation());
	}
}

void AOPAG_RoomManager::EnterRoom()
{
	if (!bHasBroadcastedEntry && !bIsStartingRoom)
	{
		bHasBroadcastedEntry = true;
		bIsAwaitingDifficultyUpdate = true;
		const UOPAG_GameInstance* GI = GetGameInstance<UOPAG_GameInstance>();
		if (GI != nullptr)
		{
			GI->GetEventManager()->RoomEntered.Broadcast();
		}
	}

	if (bIsRoomCleared)
	{
		DoorManager->OpenDoors();
		return;
	}


	DoorManager->CloseDoors();
}

void AOPAG_RoomManager::RoomCleared()
{
	bIsRoomCleared = true;
	DoorManager->OpenDoors();
	const UOPAG_GameInstance* GI = GetGameInstance<UOPAG_GameInstance>();
	if (GI != nullptr)
	{
		GI->GetEventManager()->RoomCleared.Broadcast();
	}
}

void AOPAG_RoomManager::OnStageDifficultyUpdated()
{
	if (!bIsAwaitingDifficultyUpdate) return;
	bIsAwaitingDifficultyUpdate = false;

	LootManager->GenerateLoot(0);

	if (EnemyManager->GetSpawnerCount() == 0)
	{
		RoomCleared();
		return;
	}

	EnemyManager->SpawnWave();
}