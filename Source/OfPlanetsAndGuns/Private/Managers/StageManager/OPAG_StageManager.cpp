// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/StageManager/OPAG_StageManager.h"

#include "Characters/Player/OPAG_Player.h"
#include "Engine/LevelStreamingDynamic.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Managers/OPAG_GameInstance.h"
#include "Managers/OPAG_EventManagerSubsystem.h"
#include "Managers/RoomManager/OPAG_RoomManager.h"
#include "Rooms/OPAG_RoomAttachPoint.h"
#include "Rooms/OPAG_RoomStreamingVolume.h"
#include "Util/OPAG_UtilityFunctionLibrary.h"

// Sets default values
AOPAG_StageManager::AOPAG_StageManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AOPAG_StageManager::StartStageGeneration(const FOPAG_StageGeneration GenData)
{
	if (!bOverrideGenData) BaseGenerationData = GenData;

	LoadDataTables();
	AddMoreRoomsFromListToHitWeightRequirements();

	RoomsToAppend.Empty();
	uint8 PreviousRoomTurn = 0;
	StartingRoom = -1;
	FinalRoom = -1;
	SafeRoom = -1;
	TotalWeight = 0;

	RoomCounter = 0;

	//loop until the weight has reached the target
	if (BaseGenerationData.StageType == EStageType::Stage)
	{
		while (BaseGenerationData.TargetWeight > TotalWeight)
		{
			FindRoomToSpawn(RoomsToAppend.Num() > 0 ? RoomsToAppend[0] : -1, PreviousRoomTurn);
		}
	}
	else
	{
		TotalWeight = BaseGenerationData.TargetWeight;
		FindRoomToSpawn(-1, PreviousRoomTurn);
	}

	LoadingProgress->Broadcast(LoadingProgress_Pathing);

	UOPAG_UtilityFunctionLibrary::ScreenLogWithSeverityColor(this, FString::Printf(
		TEXT("Spawning Persistent Rooms, Count: %i"),
		RoomLoadingCounter), EMessageSeverity::Info);

	//Spawn first room
	bool bSuccess;
	ULevelStreamingDynamic* Level = 
		ULevelStreamingDynamic::LoadLevelInstanceBySoftObjectPtr(this,
		StagePathing[StartingRoom]->CurrentRoom->RoomOffset.Room,
		FVector::ZeroVector, FRotator::ZeroRotator, bSuccess);

	StagePathing[StartingRoom]->CurrentRoom->LevelStreaming = Level;
}

void AOPAG_StageManager::LoadDataTables()
{
	const FString Context;
	if (BaseGenerationData.RoomTable) BaseGenerationData.RoomTable->GetAllRows(Context, Rooms);
	checkf(Rooms.Num() > 0, TEXT("Room table can't be empty"));
	if (BaseGenerationData.BridgeTable) BaseGenerationData.BridgeTable->GetAllRows(Context, Bridges);
	checkf(Bridges.Num() > 0, TEXT("Bridge table can't be empty"));
	if (BaseGenerationData.DoubleRoomChanceTable) BaseGenerationData.DoubleRoomChanceTable->GetAllRows(Context, DoubleExitChances);
}

void AOPAG_StageManager::AddMoreRoomsFromListToHitWeightRequirements()
{
	if (BaseGenerationData.StageType != EStageType::Stage) return;

	int32 CurrentWeight = 0;
	for (const FOPAG_RoomOffset* Room : Rooms)
	{
		CurrentWeight += Room->Weight;
	}

	//exit early if we don't need more rooms
	if (CurrentWeight >= BaseGenerationData.TargetWeight)
		return;

	UOPAG_UtilityFunctionLibrary::ScreenLogWithSeverityColor(this, FString::Printf(
		TEXT("Not Enough rooms in Data table. Missing weight of %i to reach target weight"),
		BaseGenerationData.TargetWeight - CurrentWeight), EMessageSeverity::Error);

	TArray<FOPAG_RoomOffset*> NonStartRooms;
	TArray<FOPAG_RoomOffset*> StraightRooms;
	FilterStartingRooms(Rooms, NonStartRooms, false);
	FilterRoomsByExitType(NonStartRooms, StraightRooms,
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

void AOPAG_StageManager::FindRoomToSpawn(const int32 Previous, uint8& PreviousRoomTurn)
{
	FOPAG_StreamingRoom* RandomRoom = new FOPAG_StreamingRoom;
	FOPAG_RoomOffset* RandomRoomOffset;
	const bool bIsNotFirst = Previous != -1;
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
	RoomCounter++;

	if (bIsNotFirst)
	{
		StagePathing[Previous]->NextRoomIDs.Add(RandomRoom->ID);
	}


	const auto AppendIndex = RoomsToAppend.Find(Previous);
	if (AppendIndex >= 0)
		RoomsToAppend.RemoveAt(AppendIndex);

	if (RandomRoom->ID >= SpecialRoomID)
	{
		UOPAG_UtilityFunctionLibrary::ScreenLogWithSeverityColor(this, 
			"Pretending to remove room from special table", EMessageSeverity::Info);
	}
	//else
	{
		const auto OffsetIndex = Rooms.Find(RandomRoomOffset);
		if (OffsetIndex >= 0)
			Rooms.RemoveAt(OffsetIndex);
		if (RandomRoom->ID < SpecialRoomID) //check is temp for special room
		{
			TotalWeight += RandomRoom->RoomOffset.Weight;
			RoomLoadingCounter++;
			if (RandomRoom->RoomOffset.ExitCount > 1) DoubleExitRoomCounter++;
			if (TotalWeight >= BaseGenerationData.TargetWeight) FinalRoom = RandomRoom->ID;
		}
	}

	if (StartingRoom == -1) StartingRoom = RandomRoom->ID;


	if (RandomRoom->ID != FinalRoom && RandomRoom->RoomOffset.ExitCount > 0)
	{
		if (RandomRoom->ID < SpecialRoomID)
		{
			AddRandomBridge(PathingItem);
			if (RandomRoom->RoomOffset.ExitCount > 1)
			{
				if (SafeRoom < 0) //can add more check for some rng
				{
					SafeRoom = RandomRoom->ID;
				}
				else if (SpecialRoomBridges.Num() == 0) //Not deleting any endbridges from this array. this means that this will be true only at the first entry.
				{
					SpecialRoomBridges.Add(AddRandomBridge(PathingItem, true));
				}
			}
		}
	}
}

void AOPAG_StageManager::FilterRoomsByExitType(const TArray<FOPAG_RoomOffset*>& RoomList,
	TArray<FOPAG_RoomOffset*>& FilteredRooms, const uint8 LastRoomTurn) const
{
	for (FOPAG_RoomOffset* Room : RoomList)
	{
		//bitwise & to find rooms that don't have the same turn (example both having left exit)
		if ((Room->ExitSides & LastRoomTurn) == 0)
		{
			FilteredRooms.Add(Room);
		}
	}
}

void AOPAG_StageManager::FilterRoomsByRemainingWeight(const TArray<FOPAG_RoomOffset*>& RoomList,
	TArray<FOPAG_RoomOffset*>& FilteredRooms) const
{
	for (FOPAG_RoomOffset* Room : RoomList)
	{
		if (Room->Weight <= BaseGenerationData.TargetWeight - TotalWeight)
		{
			FilteredRooms.Add(Room);
		}
	}
}

void AOPAG_StageManager::FilterDoubleExitRooms(const TArray<FOPAG_RoomOffset*>& RoomList,
	TArray<FOPAG_RoomOffset*>& FilteredRooms, const bool bHasDoubleExits) const
{
	for (FOPAG_RoomOffset* Room : RoomList)
	{
		if ((Room->ExitCount > 1 && bHasDoubleExits) ||
			(Room->ExitCount <= 1 && !bHasDoubleExits))
		{
			FilteredRooms.Add(Room);
		}
	}
}

void AOPAG_StageManager::FilterStartingRooms(const TArray<FOPAG_RoomOffset*>& RoomList,
	TArray<FOPAG_RoomOffset*>& FilteredRooms, const bool bIsStartingRoom) const
{
	for (FOPAG_RoomOffset* Room : RoomList)
	{
		switch (Room->RoomType) { case ERoomType::Normal: 
			if (!bIsStartingRoom) FilteredRooms.Add(Room);
			break;
		case ERoomType::SpaceshipStart: 
			if (bIsStartingRoom && BaseGenerationData.StageIndex == 0) FilteredRooms.Add(Room);
			break;
		case ERoomType::TeleporterStart: 
			if (bIsStartingRoom && BaseGenerationData.StageIndex > 0) FilteredRooms.Add(Room);
			break;
		default: ;
		}
	}
}

FOPAG_RoomOffset* AOPAG_StageManager::GetRandomRoom(bool bIsNotFirst, const uint8& PreviousRoomTurn)
{
	TArray<FOPAG_RoomOffset*> InputRooms = Rooms;
	TArray<FOPAG_RoomOffset*> FilteredRooms;

	if (!bIsNotFirst)
	{
		FilterStartingRooms(Rooms, FilteredRooms, true);
		if (FilteredRooms.Num() == 0)
		{
			UOPAG_UtilityFunctionLibrary::ScreenLogWithSeverityColor(this,
				FString::Printf(TEXT("%i - StartingRoom filter was empty!"), StagePathing.Num()),
				EMessageSeverity::Error);
		}
		else
		{
			InputRooms = FilteredRooms;
			FilteredRooms.Empty();

			//also get rid of starting rooms for future room filters
			FilterStartingRooms(Rooms, FilteredRooms, false);
			if (FilteredRooms.Num() == 0)
			{
				UOPAG_UtilityFunctionLibrary::ScreenLogWithSeverityColor(this,
					FString::Printf(TEXT("%i - All rooms are starting rooms!"), StagePathing.Num()),
					EMessageSeverity::Error);
			}
			else
			{
				Rooms = FilteredRooms;
			}
		}
		FilteredRooms.Empty();
	}

	FilterDoubleExitRooms(InputRooms, FilteredRooms, bIsNotFirst && IsRoomDouble());
	if (FilteredRooms.Num() == 0)
	{
		UOPAG_UtilityFunctionLibrary::ScreenLogWithSeverityColor(this, 
			FString::Printf(TEXT("%i - DoubleRoom filter was empty!"), StagePathing.Num()),
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
		UOPAG_UtilityFunctionLibrary::ScreenLogWithSeverityColor(this, 
			FString::Printf(TEXT("%i - ExitType filter was empty!"), StagePathing.Num()),
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
		UOPAG_UtilityFunctionLibrary::ScreenLogWithSeverityColor(this, 
			FString::Printf(TEXT("%i - Weight filter was empty!"), StagePathing.Num()),
			EMessageSeverity::Error);
		FilteredRooms = InputRooms;
	}

	return FilteredRooms[FMath::RandRange(0, FilteredRooms.Num() - 1)];
}

FOPAG_RoomOffset* AOPAG_StageManager::GetRandomSpecialRoom(const uint8& PreviousRoomTurn)
{
	//should have special room logic here instead of call to random room
	UOPAG_UtilityFunctionLibrary::ScreenLogWithSeverityColor(this, 
		"Pretending to get special room", EMessageSeverity::Info);
	return GetRandomRoom(true, PreviousRoomTurn);
}

bool AOPAG_StageManager::IsRoomDouble()
{
	if (DoubleExitRoomCounter >= BaseGenerationData.MaxDoubleExitRooms) return false;

	float Chance = 0.f;
	FOPAG_DoubleExitChance* Item;
	DoubleExitChances.Sort([](const FOPAG_DoubleExitChance Lhs, const FOPAG_DoubleExitChance Rhs)
	{
		return Lhs.MinCountForChance > Rhs.MinCountForChance;
	});
	for (int i = 0; i < DoubleExitChances.Num(); ++i)
	{
		Item = DoubleExitChances[i];

		if (Item->MinCountForChance > DoubleExitRoomCounter)
			continue;

		//check if we are guaranteed a double room
		if (Item->GuaranteedSpawnRoomIndex >= 0 && Item->GuaranteedSpawnRoomIndex <= RoomLoadingCounter)
		{
			Chance = 100.f;
			break;
		}

		Chance = Item->Chance;
		break;

		
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

	const FOPAG_RoomOffset* RandomBridgeOffset = Bridges[FMath::RandRange(0, Bridges.Num()-1)];

	RandomBridge->CurrentRoom = new FOPAG_StreamingRoom;
	RandomBridge->CurrentRoom->ID = RoomLoadingCounter + (SecondaryExit ? BridgeID * 2 : BridgeID);
	RandomBridge->CurrentRoom->RoomOffset = *RandomBridgeOffset;
	//RandomBridge->CurrentRoom->LevelStreaming = nullptr;
	RandomBridge->PrevRoomID = Room->CurrentRoom->ID;

	StagePathing.Add(RandomBridge->CurrentRoom->ID, RandomBridge);

	Room->NextRoomIDs.Add(RandomBridge->CurrentRoom->ID);
	RoomsToAppend.Add(RandomBridge->CurrentRoom->ID);
	return RandomBridge->CurrentRoom->ID;
}

void AOPAG_StageManager::FinishedLoadingStage() const
{
	const UOPAG_GameInstance* GI = Cast<UOPAG_GameInstance>(GetGameInstance());
	if (GI)
	{
		GI->GetEventManager()->StageLoaded.Broadcast(RoomCounter);
		FTransform StartTransform = StagePathing[StartingRoom]->CurrentRoom->RoomOffset.Offset;
		const FTransform LevelTransform = StagePathing[StartingRoom]->CurrentRoom->LevelStreaming->LevelTransform;
		StartTransform.AddToTranslation(LevelTransform.GetTranslation());
		GI->GetEventManager()->PlacePlayerAtStart.Broadcast(StartTransform);
	}
}

void AOPAG_StageManager::OnRoomSpawned(AOPAG_RoomManager* const RoomManager)
{
	if (RoomManagers.Contains(RoomManager)) return;

	LoadingProgress->Broadcast(LoadingProgress_Pathing +
		(LoadingProgress_Rooms- LoadingProgress_Pathing) / StagePathing.Num() * RoomManagers.Num());

	FOPAG_StagePathing* CurrentNode = FindPathingToAssign(RoomManager->GetLevel());
	if (CurrentNode == nullptr)
	{
		UOPAG_UtilityFunctionLibrary::ScreenLogWithSeverityColor(this, 
			"No Pathing found for RoomManager under level " +
			RoomManager->GetLevel()->GetFullName(), EMessageSeverity::Error);
		return;
	}

	if (StagePathing.Num() -1 > RoomManagers.Num())
	{
		RoomManagers.Add(RoomManager);
		RoomManager->AssignedRoomID = CurrentNode->CurrentRoom->ID;

		TArray<AOPAG_RoomAttachPoint*> Exits = RoomManager->Exits;
		UOPAG_UtilityFunctionLibrary::ShuffleArray(Exits);
		for (int32 i = 0; i < CurrentNode->NextRoomIDs.Num(); ++i)
		{
			if (i < Exits.Num())
			{
				checkf(Exits[i] != nullptr, TEXT("Exit cannot be configured as nullptr. Fix exits for %s"), 
					*RoomManager->GetLevel()->GetFullName());
				if (!Exits[i]) break;
				AddRoom(CurrentNode->NextRoomIDs[i], Exits[i]);
			}
			else break;
		}
	}
	else if (!bVolumesAssigned)
	{
		RoomManagers.Add(RoomManager);
		RoomManager->AssignedRoomID = CurrentNode->CurrentRoom->ID;
		PostRoomLoadingActions();
	}
}

FOPAG_StagePathing* AOPAG_StageManager::FindPathingToAssign(const ULevel* Level)
{
	TArray<uint32> Keys;
	StagePathing.GetKeys(Keys);
	FOPAG_StagePathing* Pathing = nullptr;
	for (const auto Key : Keys)
	{
		if (StagePathing[Key]->CurrentRoom->LevelStreaming != nullptr && 
			StagePathing[Key]->CurrentRoom->LevelStreaming->GetLoadedLevel()->GetFullName() == Level->GetFullName())
		{
			Pathing = StagePathing[Key];
			break;
		}
	}
	return Pathing;
}

void AOPAG_StageManager::AddRoom(const int32 ID, AOPAG_RoomAttachPoint* AttachPoint)
{
	const FOPAG_StagePathing* Pathing = StagePathing[ID];
	if (Pathing == nullptr) return;

	AttachPoint->bIsOpen = true;

	FTransform AppliedOffset;
	GetAppliedTransform(AttachPoint->GetActorTransform(),
		Pathing->CurrentRoom->RoomOffset.Offset, AppliedOffset);

	bool bSuccess;
	ULevelStreamingDynamic* Level = 
		ULevelStreamingDynamic::LoadLevelInstanceBySoftObjectPtr(this,
		Pathing->CurrentRoom->RoomOffset.Room,
		AppliedOffset.GetLocation(),
			AppliedOffset.Rotator(),
		bSuccess);	

	Pathing->CurrentRoom->LevelStreaming = Level;
}

void AOPAG_StageManager::GetAppliedTransform(const FTransform& PreviousEnd, const FTransform& NextStart, 
	FTransform& OutAppliedTransform)
{
	const FRotator NewRotation = PreviousEnd.Rotator() + NextStart.Rotator().GetInverse();

	const FVector NewOffset = PreviousEnd.GetLocation() - NewRotation.RotateVector(NextStart.GetLocation());

	OutAppliedTransform = FTransform(NewRotation, NewOffset, PreviousEnd.GetScale3D());
}

void AOPAG_StageManager::PostRoomLoadingActions()
{
	LoadGeometryRooms();
	LoadingProgress->Broadcast(LoadingProgress_Geometry);
	AssignVolumes();
	LoadingProgress->Broadcast(LoadingProgress_Volumes);

	PostGenerationActions();
}

void AOPAG_StageManager::LoadGeometryRooms()
{
	UOPAG_UtilityFunctionLibrary::ScreenLogWithSeverityColor(this, 
		"Spawning Geometry Rooms", EMessageSeverity::Info);

	for (const AOPAG_RoomManager* RoomManager : RoomManagers)
	{
		AddGeometryRoom(RoomManager->AssignedRoomID, RoomManager->GeometryLevel);
	}
}

void AOPAG_StageManager::AddGeometryRoom(const int32 ID, const TSoftObjectPtr<UWorld> GeometryLevel)
{
	const FOPAG_StagePathing* Pathing = StagePathing[ID];
	if (Pathing == nullptr) return;

	bool bSuccess;
	ULevelStreamingDynamic* Level = 
		ULevelStreamingDynamic::LoadLevelInstanceBySoftObjectPtr(this,
		GeometryLevel,
		Pathing->CurrentRoom->LevelStreaming->LevelTransform.GetLocation(),
		Pathing->CurrentRoom->LevelStreaming->LevelTransform.GetRotation().Rotator(),
		bSuccess);

	Pathing->CurrentRoom->LevelStreaming = Level;
	Level->bShouldBlockOnLoad = true;
}

void AOPAG_StageManager::AssignVolumes()
{
	if (bVolumesAssigned) return;

	UOPAG_UtilityFunctionLibrary::ScreenLogWithSeverityColor(this, 
		"Assigning Volumes", EMessageSeverity::Info);

	for (const AOPAG_RoomManager* RoomManager : RoomManagers)
	{
		RoomStreamingVolumes.Add(RoomManager->AssignedRoomID, RoomManager->Volume);
	}

	TArray<uint32> VolumeKeys;
	RoomStreamingVolumes.GetKeys(VolumeKeys);
	for (const auto Key : VolumeKeys)
	{

		AddLevelsToVolume(RoomStreamingVolumes[Key], Key);
	}

	bVolumesAssigned = true;
}

void AOPAG_StageManager::AddLevelsToVolume(AOPAG_RoomStreamingVolume* Volume, const uint32 ID)
{
	if (!StagePathing.Contains(ID)) return;
	const auto Pathing = StagePathing[ID];

	Volume->StreamingLevels.Add(Pathing->CurrentRoom->LevelStreaming);

	AddLevelsAboveVolume(Pathing->PrevRoomID, Volume->StreamingLevels);
	AddLevelsBelowVolume(Pathing->NextRoomIDs, Volume->StreamingLevels);
}

void AOPAG_StageManager::AddLevelsAboveVolume(const int32 ID, TArray<ULevelStreamingDynamic*>& StreamingVolumes)
{
	int32 CurrentID = ID;
	for (int32 i = 0; i < BaseGenerationData.FarthestLoad; ++i)
	{
		if (!StagePathing.Contains(CurrentID)) continue;
		const auto Pathing = StagePathing[CurrentID];
		StreamingVolumes.Add(Pathing->CurrentRoom->LevelStreaming);
		CurrentID = Pathing->PrevRoomID;
	}
}

void AOPAG_StageManager::AddLevelsBelowVolume(const TArray<int32> IDs, TArray<ULevelStreamingDynamic*>& StreamingVolumes)
{
	TArray<int32> CurrentIDs = IDs;
	TArray<int32> NextSet;

	for (int32 i = 0; i < BaseGenerationData.FarthestLoad; ++i)
	{
		for (const int32 CurrentID : CurrentIDs)
		{
			if (!StagePathing.Contains(CurrentID)) continue;
			const auto Pathing = StagePathing[CurrentID];
			StreamingVolumes.Add(Pathing->CurrentRoom->LevelStreaming);
			NextSet.Append(Pathing->NextRoomIDs);
		}
		CurrentIDs = NextSet;
		NextSet.Empty();
	}
}

void AOPAG_StageManager::PostGenerationActions()
{
	AssignClosedExits();
	UnloadLevels();
	LoadingProgress->Broadcast(LoadingProgress_Full);
	GetWorldTimerManager().SetTimerForNextTick(FTimerDelegate::CreateLambda([this]()
	{
		FinishedLoadingStage();
	}));
}

void AOPAG_StageManager::AssignClosedExits()
{
	for (const AOPAG_RoomManager* RoomManager : RoomManagers)
	{
		const FOPAG_StagePathing* Pathing = StagePathing[RoomManager->AssignedRoomID];
		if (Pathing == nullptr) continue;

		RoomManager->CloseExits(Pathing->CurrentRoom->ID == FinalRoom, Pathing->CurrentRoom->ID == SafeRoom);
	}
}

void AOPAG_StageManager::UnloadLevels()
{
	if (bDisableLevelUnloading || !bVolumesAssigned) return;

	TArray<ULevelStreamingDynamic*> LevelsToUnload;
	TArray<uint32> Keys;
	StagePathing.GetKeys(Keys);
	for (const uint32 Key : Keys)
	{
		LevelsToUnload.Add(StagePathing[Key]->CurrentRoom->LevelStreaming);
	}

	Keys.Empty();
	RoomStreamingVolumes.GetKeys(Keys);
	const AActor* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawnOrSpectator();
	for (const uint32 Key : Keys)
	{
		if (RoomStreamingVolumes[Key]->IsOverlappingActor(PlayerPawn))
		{
			for (auto StreamingLevel : RoomStreamingVolumes[Key]->StreamingLevels)
			{
				LevelsToUnload.Remove(StreamingLevel);
			}
		}
	}

	for (ULevelStreamingDynamic* ToUnload : LevelsToUnload)
	{
		ToUnload->SetShouldBeVisible(false);
	}
}

void AOPAG_StageManager::PreInitializeComponents()
{
	Super::PreInitializeComponents();
	const UOPAG_GameInstance* GI = Cast<UOPAG_GameInstance>(GetGameInstance());
	if (GI)
	{
		GI->GetEventManager()->StageManagerInit.Broadcast(GetWorld());
	}
}

void AOPAG_StageManager::LoadLevels(AOPAG_RoomStreamingVolume* Volume)
{
	for (ULevelStreamingDynamic* StreamingLevel : Volume->StreamingLevels)
	{
		if (!StreamingLevel->ShouldBeVisible())
		{
			StreamingLevel->SetShouldBeVisible(true);
		}
	}
}



// Called when the game starts or when spawned
void AOPAG_StageManager::BeginPlay()
{
	Super::BeginPlay();
	const UOPAG_GameInstance* GI = Cast<UOPAG_GameInstance>(GetGameInstance());
	if (GI)
	{
		UOPAG_EventManagerSubsystem* const EventManager = GI->GetEventManager();
		EventManager->GenerateStage.AddDynamic(this, &AOPAG_StageManager::StartStageGeneration);
		EventManager->RoomSpawned.AddDynamic(this, &AOPAG_StageManager::OnRoomSpawned);
		EventManager->EnteredRoomStreamingVolume.AddDynamic(this, &AOPAG_StageManager::LoadLevels);
		EventManager->LeftRoomStreamingVolume.AddDynamic(this, &AOPAG_StageManager::UnloadLevels);
		LoadingProgress = &(EventManager->LoadingProgress);
		EventManager->StageManagerReady.Broadcast();
	}
}

// Called every frame
void AOPAG_StageManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}