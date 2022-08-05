// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OPAG_StageGeneration.h"
#include "OPAG_StagePathing.h"
#include "GameFramework/Actor.h"
#include "Managers/OPAG_EventManagerSubsystem.h"
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


	FLoadingProgress* LoadingProgress;

	// Room Data

	TArray<FOPAG_RoomOffset*> Rooms;
	TArray<FOPAG_RoomOffset*> Bridges;
	TArray<FOPAG_DoubleExitChance*> DoubleExitChances;

	// Room Streaming

	TMap<uint32, AOPAG_RoomStreamingVolume*> RoomStreamingVolumes;
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

	int32 RoomCounter;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FOPAG_StageGeneration BaseGenerationData;

	// Overrides the StageGeneration data from the Game Manager
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bOverrideGenData;

	// Debug flag to disable level unloading
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay)
		bool bDisableLevelUnloading;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftClassPtr<APawn> Pawn;
	// Sets default values for this actor's properties
	AOPAG_StageManager();

	static constexpr uint32 BridgeID = 100;
	static constexpr uint32 SpecialRoomID = 900;

	static constexpr float LoadingProgress_Pathing = 0.2f;
	static constexpr float LoadingProgress_Rooms = 0.8f;
	static constexpr float LoadingProgress_Geometry = 0.9f;
	static constexpr float LoadingProgress_Volumes = 0.95f;
	static constexpr float LoadingProgress_Full = 1.f;

private:
	UFUNCTION()
	void StartStageGeneration(FOPAG_StageGeneration GenData);
	void LoadDataTables();
	void AddMoreRoomsFromListToHitWeightRequirements();
	void FindRoomToSpawn(const int32 Previous, uint8& PreviousRoomTurn);

	void FilterRoomsByExitType(const TArray<FOPAG_RoomOffset*>& RoomList,
		TArray<FOPAG_RoomOffset*>& FilteredRooms, uint8 LastRoomTurn) const;
	void FilterRoomsByRemainingWeight(const TArray<FOPAG_RoomOffset*>& RoomList, 
		TArray<FOPAG_RoomOffset*>& FilteredRooms) const;
	void FilterDoubleExitRooms(const TArray<FOPAG_RoomOffset*>& RoomList, 
		TArray<FOPAG_RoomOffset*>& FilteredRooms, const bool bHasDoubleExits) const;
	void FilterStartingRooms(const TArray<FOPAG_RoomOffset*>& RoomList,
		TArray<FOPAG_RoomOffset*>& FilteredRooms, const bool bIsStartingRoom) const;

	FOPAG_RoomOffset* GetRandomRoom(bool bIsNotFirst, const uint8& PreviousRoomTurn);
	FOPAG_RoomOffset* GetRandomSpecialRoom(const uint8& PreviousRoomTurn);
	bool IsRoomDouble();

	static void UpdateRoomTurnBitmask(uint8& Previous, uint8 Next);

	int32 AddRandomBridge(FOPAG_StagePathing* Room, bool SecondaryExit = false);

	void FinishedLoadingStage() const;
	UFUNCTION()
	void OnRoomSpawned(AOPAG_RoomManager* const RoomManager);

	FOPAG_StagePathing* FindPathingToAssign(const ULevel* Level);
	void AddRoom(int32 ID, AOPAG_RoomAttachPoint* AttachPoint);
	static void GetAppliedTransform(const FTransform& PreviousEnd, const FTransform& NextStart, 
		FTransform& OutAppliedTransform);

	void PostRoomLoadingActions();

	void LoadGeometryRooms();
	void AddGeometryRoom(int32 ID, TSoftObjectPtr<UWorld> GeometryLevel);

	void AssignVolumes();
	void AddLevelsToVolume(AOPAG_RoomStreamingVolume* Volume, uint32 ID);
	void AddLevelsAboveVolume(int32 ID, TArray<ULevelStreamingDynamic*>& StreamingVolumes);
	void AddLevelsBelowVolume(TArray<int32> IDs, TArray<ULevelStreamingDynamic*>& StreamingVolumes);

	void PostGenerationActions();

	void AssignClosedExits();
	void ReplacePawn() const;
	UFUNCTION()
	void LoadLevels(AOPAG_RoomStreamingVolume* const Volume);
	UFUNCTION()
	void UnloadLevels();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};