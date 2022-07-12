// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
//#include "Managers/StageManager/OPAG_StageGeneration.h"
#include "OPAG_EventManagerSubsystem.generated.h"

/**
 * 
 */

//class AOPAG_RoomStreamingVolume;
//class AOPAG_RoomManager;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStageManagerReady);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGenerateStage, FOPAG_StageGeneration, StageGenData);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRoomSpawned, AOPAG_RoomManager* const, RoomManager);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStageLoaded);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRoomCleared);

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEnteredRoomStreamingVolume, AOPAG_RoomStreamingVolume* const, Volume);

UCLASS()
class OFPLANETSANDGUNS_API UOPAG_EventManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable, Category = StageManager)
		FStageManagerReady StageManagerReady;
	//UPROPERTY(BlueprintAssignable, Category = StageManager)
		//FGenerateStage GenerateStage;
	//UPROPERTY(BlueprintAssignable, Category = StageManager)
		//FRoomSpawned RoomSpawned;
	UPROPERTY(BlueprintAssignable, Category = StageManager)
		FStageLoaded StageLoaded;

	UPROPERTY(BlueprintAssignable, Category = RoomManager)
		FRoomCleared RoomCleared;

	//UPROPERTY(BlueprintAssignable, Category = RoomManager)
		//FEnteredRoomStreamingVolume EnteredRoomStreamingVolume;

};
