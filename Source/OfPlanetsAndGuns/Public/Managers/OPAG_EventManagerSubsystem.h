#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Managers/StageManager/OPAG_StageGeneration.h"
#include "InputActionValue.h"
#include "OPAG_EventManagerSubsystem.generated.h"

class AOPAG_RoomStreamingVolume;
class AOPAG_RoomManager;
class AOPAG_Enemy;

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
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAmmoPickedUp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDashAmountChanged, int, CurrentValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDashChargeChanged, float, CurrentValue, float, MaxValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGoldChanged, int, CurrentValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBoltsChanged, int, CurrentValue);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEnemyKilled, const AOPAG_Enemy* const, DeadEnemy);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMoveForward, const FInputActionValue&, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMoveRight, const FInputActionValue&, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLookUp, const FInputActionValue&, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTurn, const FInputActionValue&, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FJumping);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStopJumping);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSwapGuns);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FReloadGun);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FShoot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInteractWithItem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSwitchToADSCamera);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FResetFPSCamera);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FActiveDash);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FActiveGunAbility);

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

	//Gun stuff
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = Gun)
		FAmmoPickedUp AmmoPickedUp;

    //Enemy stuff
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = EnemyManager)
		FEnemyKilled EnemyKilled;

	//Game stuff
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = Game)
		FPauseGame PauseGame;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = Game)
		FResumeGame ResumeGame;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = Game)
		FStartRun StartRun;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = Game)
		FQuitGame QuitGame;

	// Player Enhanced
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Enhanced Player")
	FMoveForward MoveForward;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Enhanced Player")
	FMoveForward MoveRight;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Enhanced Player")
		FLookUp Lookup;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Enhanced Player")
		FTurn Turn;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Enhanced Player")
		FShoot Shoot;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Enhanced Player")
		FSwapGuns SwapGuns;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Enhanced Player")
		FReloadGun ReloadGun;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Enhanced Player")
		FJumping Jumping;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Enhanced Player")
		FStopJumping StopJumping;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Enhanced Player")
		FInteractWithItem InteractWithItem;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Enhanced Player")
		FSwitchToADSCamera SwitchToADSCamera;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Enhanced Player")
		FResetFPSCamera ResetFPSCamera;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Enhanced Player")
		FHealth Health;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Enhanced Player")
		FActiveDash ActiveDash;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Enhanced Player")
		FActiveGunAbility ActiveGunAbility;
};
