// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OPAG_StageGeneration.h"
#include "OPAG_StagePathing.h"
#include "GameFramework/Actor.h"
#include "Managers/RoomManager/OPAG_DoubleExitChance.h"
#include "Managers/RoomManager/OPAG_RoomOffset.h"
#include "OPAG_StageManager.generated.h"

UCLASS()
class OFPLANETSANDGUNS_API AOPAG_StageManager : public AActor
{
	GENERATED_BODY()


	// Room Data

	TArray<FOPAG_RoomOffset*> Rooms;
	TArray<FOPAG_RoomOffset*> Bridges;
	TArray<FOPAG_DoubleExitChance*> DoubleExitChances;

	// Room Streaming

	TMap<int, AActor*> RoomStreaingVolumes;
	bool bVolumesAssigned;
	TArray<AActor*> RoomManagers;


	// Pathing

	int TotalWeight;
	TMap<int, FOPAG_StagePathing> StagePathing;

	//these should be in function scope
	int RoomLoadingCounter;
	int DoubleExitRoomCounter;
	TArray<int> RoomsToAppend;
	int StartingRoom;
	int LastRoomTurn;
	TArray<int> SpecialRoomBridges;

	int FinalRoom;
	int SafeRoom;

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

private:
	UFUNCTION()
	void StartStageGeneration(FOPAG_StageGeneration GenData);
	void FinishedLoadingStage();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
