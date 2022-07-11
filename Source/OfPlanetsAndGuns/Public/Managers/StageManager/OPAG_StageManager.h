// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OPAG_StageGeneration.h"
#include "OPAG_StagePathing.h"
#include "GameFramework/Actor.h"
#include "Managers/RoomManager/OPAG_DoubleExitChance.h"
#include "Managers/RoomManager/OPAG_RoomOffset.h"
#include "OPAG_StageManager.generated.h"

class AOPAG_RoomAttachPoint;
class AOPAG_RoomManager;
class AOPAG_RoomStreamingVolume;
UCLASS()
class OFPLANETSANDGUNS_API AOPAG_StageManager : public AActor
{
	GENERATED_BODY()


	// Room Data

	TArray<FOPAG_RoomOffset*> Rooms;
	TArray<FOPAG_RoomOffset*> Bridges;
	TArray<FOPAG_DoubleExitChance*> DoubleExitChances;

	// Room Streaming

	TMap<uint32, AOPAG_RoomStreamingVolume*> RoomStreaingVolumes;
	bool bVolumesAssigned;
	TArray<AOPAG_RoomManager*> RoomManagers;


	// Pathing

	int32 TotalWeight;
	TMap<uint32, FOPAG_StagePathing*> StagePathing;

	//these should be in function scope
	int32 RoomLoadingCounter;
	int32 DoubleExitRoomCounter;
	TArray<int32> RoomsToAppend;
	int32 StartingRoom;
	//uint8 LastRoomTurn;
	TArray<int32> SpecialRoomBridges;

	int32 FinalRoom;
	int32 SafeRoom;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FOPAG_StageGeneration BaseGenerationData;

	// Overrides the StageGeneration data from the Game Manager
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bOverrideGenData;

	// Debug flag to disable level unloading
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay)
		bool bDisableLevelUnloading;

	// Sets default values for this actor's properties
	AOPAG_StageManager();

	static constexpr uint32 BridgeID = 100;
	static constexpr uint32 SpecialRoomID = 900;

private:
	UFUNCTION()
	void StartStageGeneration(FOPAG_StageGeneration GenData);
	void LoadDataTables();
	void AddMoreRoomsFromListToHitWeightRequirements();
	void FindRoomToSpawn(int32 Previous, uint8& PreviousRoomTurn);

	void FilterRoomsByExitType(const TArray<FOPAG_RoomOffset*>& RoomList,
		TArray<FOPAG_RoomOffset*>& FilteredRooms, uint8 LastRoomTurn);
	void FilterRoomsByRemainingWeight(const TArray<FOPAG_RoomOffset*>& RoomList, 
		TArray<FOPAG_RoomOffset*>& FilteredRooms);
	void FilterDoubleExitRooms(const TArray<FOPAG_RoomOffset*>& RoomList, 
		TArray<FOPAG_RoomOffset*>& FilteredRooms, bool bHasDoubleExits);

	FOPAG_RoomOffset* GetRandomRoom(bool bIsNotFirst, uint8& PreviousRoomTurn);
	FOPAG_RoomOffset* GetRandomSpecialRoom(uint8& PreviousRoomTurn);
	bool IsRoomDouble();

	void UpdateRoomTurnBitmask(uint8& Previous, uint8 Next);

	int32 AddRandomBridge(FOPAG_StagePathing* Room, bool SecondaryExit = false);

	void FinishedLoadingStage();
	void LoadMaps(AOPAG_RoomStreamingVolume* const Volume);
	void OnRoomSpawned(AOPAG_RoomManager* const RoomManager);

	FOPAG_StagePathing* FindPathingToAssign(const ULevel* Level);
	void AddRoom(int32 ID, AOPAG_RoomAttachPoint* AttachPoint);

	void LoadGeometryRooms();
	void AddGeometryRoom(int32 ID, FName GeometryName);

	void ScreenLog(FString Message, EMessageSeverity::Type Severity);

	template<typename T>
	void ShuffleArray(TArray<T>& Array);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};