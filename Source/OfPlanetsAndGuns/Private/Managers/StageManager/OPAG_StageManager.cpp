// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/StageManager/OPAG_StageManager.h"

#include "Managers/OPAG_GameInstance.h"
#include "Managers/OPAG_EventManagerSubsystem.h"
#include "Managers/RoomManager/OPAG_RoomManager.h"
#include "Rooms/OPAG_RoomAttachPoint.h"
#include "Rooms/OPAG_RoomStreamingVolume.h"

// Sets default values
AOPAG_StageManager::AOPAG_StageManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AOPAG_StageManager::StartStageGeneration(FOPAG_StageGeneration GenData)
{
	LoadDataTables();
	AddMoreRoomsFromListToHitWeightRequirements();

	RoomsToAppend.Empty();
	uint8 PreviousRoomTurn = 0;

	//loop until the weight has reached the target
	while (BaseGenerationData.TargetWeight > TotalWeight)
	{
		FindRoomToSpawn(RoomsToAppend.Num() > 0 ? RoomsToAppend[0] : -1, PreviousRoomTurn);
	}

	ScreenLog(FString::Printf(
		TEXT("Spawning Persistent Rooms, Count: %i"),
		RoomLoadingCounter), EMessageSeverity::Info);

	//Spawn first room
	bool bSuccess;
	ULevelStreamingDynamic* Level = ULevelStreamingDynamic::LoadLevelInstance(this,
		StagePathing[StartingRoom]->CurrentRoom->RoomOffset.RoomName.ToString(),
		FVector::ZeroVector, FRotator::ZeroRotator, bSuccess);

	StagePathing[StartingRoom]->CurrentRoom->LevelStreaming = Level;
}

void AOPAG_StageManager::LoadDataTables()
{
	const FString Context;
	BaseGenerationData.RoomTable->GetAllRows(Context, Rooms);
	BaseGenerationData.BridgeTable->GetAllRows(Context, Bridges);
	BaseGenerationData.DoubleRoomChanceTable->GetAllRows(Context, DoubleExitChances);
}

void AOPAG_StageManager::AddMoreRoomsFromListToHitWeightRequirements()
{
	int32 CurrentWeight = 0;
	for (const FOPAG_RoomOffset* Room : Rooms)
	{
		CurrentWeight += Room->Weight;
	}

	//exit early if we don't need more rooms
	if (CurrentWeight >= BaseGenerationData.TargetWeight)
		return;

	ScreenLog(FString::Printf(
		TEXT("Not Enough rooms in Data table. Missing weight of %i to reach target weight"),
		BaseGenerationData.TargetWeight - CurrentWeight), EMessageSeverity::Error);

	TArray<FOPAG_RoomOffset*> StraightRooms;
	FilterRoomsByExitType(Rooms, StraightRooms, 
		static_cast<uint8>(ERoomExits::Left | ERoomExits::Right));

	//add new rooms until target + 3 (so we always have just a little bit more than needed)
	while (CurrentWeight <= BaseGenerationData.TargetWeight + 3)
	{
		auto RandItem = 
			StraightRooms[FMath::RandRange(0, StraightRooms.Num() - 1)];
		Rooms.Add(RandItem);
		CurrentWeight += RandItem->Weight;
	}
}

void AOPAG_StageManager::FindRoomToSpawn(int32 Previous, uint8& PreviousRoomTurn)
{
	FOPAG_StreamingRoom* RandomRoom = new FOPAG_StreamingRoom;
	FOPAG_RoomOffset* RandomRoomOffset;
	bool bIsNotFirst = Previous != -1;
	if (SpecialRoomBridges.Contains(Previous))
	{
		RandomRoom->ID = SpecialRoomID + RoomLoadingCounter;
		RandomRoomOffset = GetRandomSpecialRoom(PreviousRoomTurn);
	}
	else
	{
		RandomRoom->ID = RoomLoadingCounter;
		RandomRoomOffset = GetRandomRoom(bIsNotFirst, PreviousRoomTurn);
	}
	RandomRoom->RoomOffset = *RandomRoomOffset;

	UpdateRoomTurnBitmask(PreviousRoomTurn, RandomRoom->RoomOffset.ExitSides);

	FOPAG_StagePathing* PathingItem = new FOPAG_StagePathing;
	PathingItem->CurrentRoom = RandomRoom;
	PathingItem->PrevRoomID = Previous;
	StagePathing.Add(RandomRoom->ID, PathingItem);

	if (bIsNotFirst)
	{
		StagePathing[Previous]->NextRoomIDs.Add(RandomRoom->ID);
	}


	const auto appendIndex = RoomsToAppend.Find(Previous);
	if (appendIndex >= 0)
		RoomsToAppend.RemoveAt(appendIndex);

	if (RandomRoom->ID >= SpecialRoomID)
	{
		ScreenLog("Pretending to remove room from special table", EMessageSeverity::Info);
	}
	//else
	{
		const auto offsetIndex = Rooms.Find(RandomRoomOffset);
		if (offsetIndex >= 0)
			Rooms.RemoveAt(offsetIndex);
		if (RandomRoom->ID < SpecialRoomID) //check is temp for special room
		{
			TotalWeight += RandomRoom->RoomOffset.Weight;
			RoomLoadingCounter++;
			if (RandomRoom->RoomOffset.ExitCount > 1) DoubleExitRoomCounter++;
			if (TotalWeight >= BaseGenerationData.TargetWeight) FinalRoom = RandomRoom->ID;
		}
	}

	if (StartingRoom == -1) StartingRoom = RandomRoom->ID;

	if (RandomRoom->ID < SpecialRoomID) //again.. temp check
	if (RandomRoom->ID != FinalRoom && RandomRoom->RoomOffset.ExitCount > 0)
	{
		AddRandomBridge(PathingItem);
		if (RandomRoom->RoomOffset.ExitCount > 1)
		{
			//Not deleting any endbridges from this array. this means that this will be true only at the first entry.
			if (SpecialRoomBridges.Num() == 0)
			{
				SpecialRoomBridges.Add(AddRandomBridge(PathingItem, true));
			}
			else
			{
				if (SafeRoom < 0) //can add more check for some rng
				{
					SafeRoom = RandomRoom->ID;
				}
			}
		}
	}


}

void AOPAG_StageManager::FilterRoomsByExitType(const TArray<FOPAG_RoomOffset*>& RoomList,
	TArray<FOPAG_RoomOffset*>& FilteredRooms, uint8 LastRoomTurn)
{
	for (FOPAG_RoomOffset* Room : Rooms)
	{
		//bitwise & to find rooms that don't have the same turn (example both having left exit)
		if ((Room->ExitSides & LastRoomTurn) == 0)
		{
			FilteredRooms.Add(Room);
		}
	}
}

void AOPAG_StageManager::FilterRoomsByRemainingWeight(const TArray<FOPAG_RoomOffset*>& RoomList,
	TArray<FOPAG_RoomOffset*>& FilteredRooms)
{
	for (FOPAG_RoomOffset* Room : Rooms)
	{
		if (Room->Weight <= TotalWeight - BaseGenerationData.TargetWeight)
		{
			FilteredRooms.Add(Room);
		}
	}
}

void AOPAG_StageManager::FilterDoubleExitRooms(const TArray<FOPAG_RoomOffset*>& RoomList,
	TArray<FOPAG_RoomOffset*>& FilteredRooms, bool bHasDoubleExits)
{
	for (FOPAG_RoomOffset* Room : Rooms)
	{
		if ((Room->ExitCount > 1 && bHasDoubleExits) ||
			(Room->ExitCount <= 1 && !bHasDoubleExits))
		{
			FilteredRooms.Add(Room);
		}
	}
}

FOPAG_RoomOffset* AOPAG_StageManager::GetRandomRoom(bool bIsNotFirst, uint8& PreviousRoomTurn)
{
	TArray<FOPAG_RoomOffset*> InputRooms = Rooms;
	TArray<FOPAG_RoomOffset*> FilteredRooms;

	FilterDoubleExitRooms(Rooms, FilteredRooms, IsRoomDouble());
	if (FilteredRooms.Num() == 0)
	{
		ScreenLog(FString::Printf(TEXT("%i - DoubleRoom filter was empty!"), StagePathing.Num()),
			EMessageSeverity::Error);
	}
	else
	{
		InputRooms = FilteredRooms;
	}
	FilteredRooms.Empty();


	FilterRoomsByExitType(InputRooms, FilteredRooms, PreviousRoomTurn);
	if (FilteredRooms.Num() == 0)
	{
		ScreenLog(FString::Printf(TEXT("%i - ExitType filter was empty!"), StagePathing.Num()),
			EMessageSeverity::Error);
	}
	else
	{
		InputRooms = FilteredRooms;
	}
	FilteredRooms.Empty();

	FilterRoomsByRemainingWeight(InputRooms, FilteredRooms);
	if (FilteredRooms.Num() == 0)
	{
		ScreenLog(FString::Printf(TEXT("%i - Weight filter was empty!"), StagePathing.Num()),
			EMessageSeverity::Error);
		FilteredRooms = InputRooms;
	}

	return FilteredRooms[FMath::RandRange(0, FilteredRooms.Num() - 1)];
}

FOPAG_RoomOffset* AOPAG_StageManager::GetRandomSpecialRoom(uint8& PreviousRoomTurn)
{
	//should have special room logic here instead of call to random room
	ScreenLog("Pretending to get special room", EMessageSeverity::Info);
	return GetRandomRoom(true, PreviousRoomTurn);
}

bool AOPAG_StageManager::IsRoomDouble()
{
	if (DoubleExitRoomCounter >= BaseGenerationData.MaxDoubleExitRooms) return false;

	float Chance = 0.f;
	FOPAG_DoubleExitChance* item;
	for (int i = 0; i < DoubleExitChances.Num(); ++i)
	{
		item = DoubleExitChances[i];

		//check if we are guaranteed a double room
		if (item->GuaranteedSpawnRoomIndex >= 0 && item->GuaranteedSpawnRoomIndex <= RoomLoadingCounter)
		{
			Chance = 100.f;
			break;
		}

		if (item->Chance > Chance && item->MinCountForChance > DoubleExitRoomCounter)
		{
			Chance = item->Chance;
		}
	}

	return FMath::FRandRange(0.f, 100.f) <= Chance;
}

void AOPAG_StageManager::UpdateRoomTurnBitmask(uint8& Previous, const uint8 Next)
{
	if (Previous == 0)
	{
		Previous = Next;
	}
	else
	{
		uint8 BothRooms = Next ^ Previous;
		BothRooms = ~BothRooms;
		// if rooms cancelled each other out, reset the mask. otherwise keep the previous state
		if (BothRooms == 0) Previous = 0;
	}
}

int32 AOPAG_StageManager::AddRandomBridge(FOPAG_StagePathing* Room, const bool SecondaryExit)
{
	FOPAG_StagePathing* RandomBridge = new FOPAG_StagePathing;

	const FOPAG_RoomOffset* RandomBridgeOffset = Bridges[FMath::RandRange(0, Bridges.Num())];

	RandomBridge->CurrentRoom = new FOPAG_StreamingRoom;
	RandomBridge->CurrentRoom->ID = RoomLoadingCounter + (SecondaryExit ? BridgeID * 2 : BridgeID);
	RandomBridge->CurrentRoom->RoomOffset = *RandomBridgeOffset;
	RandomBridge->PrevRoomID = Room->CurrentRoom->ID;

	Room->NextRoomIDs.Add(RandomBridge->CurrentRoom->ID);
	RoomsToAppend.Add(RandomBridge->CurrentRoom->ID);
	return RandomBridge->CurrentRoom->ID;
}

void AOPAG_StageManager::FinishedLoadingStage()
{
	UOPAG_GameInstance* GI = Cast<UOPAG_GameInstance>(GetGameInstance());
	if (GI)
	{
		GI->GetEventManager()->StageLoaded.Broadcast();
	}
}

void AOPAG_StageManager::LoadMaps(AOPAG_RoomStreamingVolume* const Volume)
{
	for (ULevelStreamingDynamic* StreamingLevel : Volume->StreamingLevels)
	{
		if (!StreamingLevel->ShouldBeVisible())
		{
			StreamingLevel->SetShouldBeVisible(true);
		}
	}
}

void AOPAG_StageManager::OnRoomSpawned(AOPAG_RoomManager* const RoomManager)
{
	FOPAG_StagePathing* CurrentNode = FindPathingToAssign(RoomManager->GetLevel());
	if (CurrentNode == nullptr)
	{
		ScreenLog("No Pathing found for RoomManager under level " +
			RoomManager->GetLevel()->GetFullName(), EMessageSeverity::Error);
	}

	if (StagePathing.Num() -1 > RoomManagers.Num())
	{
		RoomManagers.Add(RoomManager);

		TArray<AOPAG_RoomAttachPoint*> Exits = RoomManager->Exits;
		ShuffleArray(Exits);
		for (int32 i = 0; i < CurrentNode->NextRoomIDs.Num(); ++i)
		{
			if (i < Exits.Num())
			{
				AddRoom(CurrentNode->NextRoomIDs[i], Exits[i]);
			}
			else break;
		}
	}
	else if (!bVolumesAssigned)
	{
		RoomManagers.Add(RoomManager);
		//load geometry
	}
}

FOPAG_StagePathing* AOPAG_StageManager::FindPathingToAssign(const ULevel* Level)
{
	TArray<uint32> Keys;
	StagePathing.GetKeys(Keys);
	for (const auto Key : Keys)
	{
		if (StagePathing[Key]->CurrentRoom->LevelStreaming->GetLoadedLevel() == Level)
		{
			return StagePathing[Key];
		}
	}
	return nullptr;
}

void AOPAG_StageManager::AddRoom(const int32 ID, AOPAG_RoomAttachPoint* AttachPoint)
{
	const FOPAG_StagePathing* Pathing = StagePathing[ID];
	if (Pathing == nullptr) return;

	AttachPoint->bIsOpen = true;

	bool bSuccess;
	ULevelStreamingDynamic* Level = ULevelStreamingDynamic::LoadLevelInstance(this,
		Pathing->CurrentRoom->RoomOffset.RoomName.ToString(),
		Pathing->CurrentRoom->RoomOffset.Offset.GetLocation(),
		Pathing->CurrentRoom->RoomOffset.Offset.GetRotation().Rotator(),
		bSuccess);

	Pathing->CurrentRoom->LevelStreaming = Level;
}

void AOPAG_StageManager::LoadGeometryRooms()
{
	ScreenLog("Spawning Geometry Rooms", EMessageSeverity::Info);

	for (const AOPAG_RoomManager* RoomManager : RoomManagers)
	{
		AddGeometryRoom(RoomManager->AssignedRoomID, RoomManager->GeometryLevelName);
	}

	//delay somehow??

}

void AOPAG_StageManager::AddGeometryRoom(int32 ID, FName GeometryName)
{
	const FOPAG_StagePathing* Pathing = StagePathing[ID];
	if (Pathing == nullptr) return;

	bool bSuccess;
	ULevelStreamingDynamic* Level = ULevelStreamingDynamic::LoadLevelInstance(this,
		GeometryName.ToString(),
		Pathing->CurrentRoom->LevelStreaming->LevelTransform.GetLocation(),
		Pathing->CurrentRoom->LevelStreaming->LevelTransform.GetRotation().Rotator(),
		bSuccess);

	Pathing->CurrentRoom->LevelStreaming = Level;
	Level->bShouldBlockOnLoad = true;
}

void AOPAG_StageManager::AssignVolumes()
{
	if (bVolumesAssigned) return;

	ScreenLog("Assigning Volumes", EMessageSeverity::Info);

	for (const AOPAG_RoomManager* RoomManager : RoomManagers)
	{
		RoomStreaingVolumes.Add(RoomManager->AssignedRoomID, RoomManager->Volume);
	}

	for (auto RoomStreamingVolume : RoomStreamingVolumes)
	{
		//add levels to volumes sdgjisdglsdhgsdg
	}

	bVolumesAssigned = true;
	//post generation actions
}

void AOPAG_StageManager::ScreenLog(FString Message, EMessageSeverity::Type Severity)
{
	if (!GEngine) return;
	typedef EMessageSeverity::Type EMST;
	GEngine->AddOnScreenDebugMessage(-1,
		5.0f,
		Severity == EMST::Warning ? FColor::Yellow : 
			(Severity == EMST::Error || Severity == EMST::CriticalError ? FColor::Red :
			FColor::Green),
		Message);
}

template<typename T>
void AOPAG_StageManager::ShuffleArray(TArray<T>& Array)
{
	if (Array.Num() > 1)
	{
		int32 LastIndex = Array.Num() - 1;
		for (int32 i = 0; i <= LastIndex; ++i)
		{
			int32 Index = FMath::RandRange(i, LastIndex);
			if (i != Index)
			{
				Array.Swap(i, Index);
			}
		}
	}
}

// Called when the game starts or when spawned
void AOPAG_StageManager::BeginPlay()
{
	Super::BeginPlay();
	UOPAG_GameInstance* GI = Cast<UOPAG_GameInstance>(GetGameInstance());
	if (GI)
	{
		UOPAG_EventManagerSubsystem* const EventManager = GI->GetEventManager();
		EventManager->GenerateStage.AddDynamic(this, &AOPAG_StageManager::StartStageGeneration);
		EventManager->RoomSpawned.AddDynamic(this, &AOPAG_StageManager::OnRoomSpawned);
		EventManager->EnteredRoomStreamingVolume.AddDynamic(this, &AOPAG_StageManager::LoadMaps);

		EventManager->StageManagerReady.Broadcast();
	}
}

// Called every frame
void AOPAG_StageManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}