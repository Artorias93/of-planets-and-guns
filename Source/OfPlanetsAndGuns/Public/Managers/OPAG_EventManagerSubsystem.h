// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Managers/StageManager/OPAG_StageGeneration.h"
#include "OPAG_EventManagerSubsystem.generated.h"

/**
 * 
 */

class AOPAG_RoomStreamingVolume;
class AOPAG_RoomManager;
class AOPAG_Enemy;
class AOPAG_ZiplineSupport;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStageManagerReady);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGenerateStage, FOPAG_StageGeneration, StageGenData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRoomSpawned, AOPAG_RoomManager* const, RoomManager);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStageLoaded);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRoomEntered);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRoomCleared);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEnteredRoomStreamingVolume, AOPAG_RoomStreamingVolume* const, Volume);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLeftRoomStreamingVolume);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLoadingProgress, float, Percent);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHealthChanged, float, CurrentValue, float, MaxValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FShieldChanged, float, CurrentValue, float, MaxValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAmmoChanged, int, CurrentValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPauseGame);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FResumeGame);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStartRun);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FQuitGame);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDashAmountChanged, int, CurrentValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDashChargeChanged, float, CurrentValue, float, MaxValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGoldChanged, int, CurrentValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBoltsChanged, int, CurrentValue);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEnemyKilled, const AOPAG_Enemy* const, DeadEnemy);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FZiplineActivated, const AOPAG_ZiplineSupport* const, ZiplineSupport);


UCLASS()
class OFPLANETSANDGUNS_API UOPAG_EventManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable, Category = StageManager)
		FStageManagerReady StageManagerReady;
	UPROPERTY(BlueprintAssignable, Category = StageManager)
		FGenerateStage GenerateStage;
	UPROPERTY(BlueprintAssignable, Category = StageManager)
		FRoomSpawned RoomSpawned;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = StageManager)
		FStageLoaded StageLoaded;

	UPROPERTY(BlueprintAssignable, Category = RoomManager)
		FRoomEntered RoomEntered;
	UPROPERTY(BlueprintAssignable, Category = RoomManager)
		FRoomCleared RoomCleared;

	UPROPERTY(BlueprintAssignable, Category = RoomManager)
		FEnteredRoomStreamingVolume EnteredRoomStreamingVolume;
	UPROPERTY(BlueprintAssignable, Category = RoomManager)
		FLeftRoomStreamingVolume LeftRoomStreamingVolume;

	UPROPERTY(BlueprintAssignable, Category = StageManager)
		FLoadingProgress LoadingProgress;

	//Player stuff
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = Player)
		FHealthChanged HealthChanged;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = Player)
		FShieldChanged ShieldChanged;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = Player)
		FAmmoChanged TotalAmmoChanged;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = Player)
		FAmmoChanged CurrentAmmoChanged;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = Player)
		FDashAmountChanged DashAmountChanged;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = Player)
		FDashChargeChanged DashChargeChanged;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = Player)
		FGoldChanged GoldChanged;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = Player)
		FBoltsChanged BoltsChanged;

    //Enemy stuff
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = EnemyManager)
		FEnemyKilled EnemyKilled;

	//Game stuff
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = Player)
		FPauseGame PauseGame;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = Player)
		FResumeGame ResumeGame;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = Player)
		FStartRun StartRun;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = Player)
		FQuitGame QuitGame;

	//interactable stuff
	//UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = Interactable)
		//FZiplineActivated ZiplineActivated;
};
