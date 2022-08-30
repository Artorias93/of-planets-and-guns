#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Managers/StageManager/OPAG_StageGeneration.h"
#include "InputActionValue.h"
#include "Weapons/Guns/OPAG_GunStats.h"
#include "Weapons/Modules/OPAG_ModuleStats.h"
#include "Characters/Player/OPAG_PlayerState.h"
#include "Characters/Player/OPAG_PlayerState.h"
#include "Abilities/OPAG_AbilityTypes.h"
#include "OPAG_EventManagerSubsystem.generated.h"

class AOPAG_RoomStreamingVolume;
class AOPAG_RoomManager;
class AOPAG_Enemy;
class AOPAG_ModularGun;
class AOPAG_ModuleBase;
class AOPAG_AbilityBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStartPlanet, int, PlanetIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLoadMainMenu);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStageManagerInit, const UWorld* const, World);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStageManagerReady);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGenerateStage, FOPAG_StageGeneration, StageGenData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRoomSpawned, AOPAG_RoomManager* const, RoomManager);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStageDifficultyUpdated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStageLoaded, int32, RoomCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlacePlayerAtStart, const FTransform, StartLocation);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStageCleared, const EStageType, NextStageType);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRoomEntered);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRoomCleared);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEnteredRoomStreamingVolume, AOPAG_RoomStreamingVolume* const, Volume);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLeftRoomStreamingVolume);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLoadingProgress, float, Percent);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRequestSavePlayerState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRequestPlayerState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSavePlayerState, FOPAG_PlayerState, PlayerState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FApplyPlayerState, const FOPAG_PlayerState&, PlayerState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerDied);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHealthChanged, float, CurrentValue, float, MaxValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FShieldChanged, float, CurrentValue, float, MaxValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAmmoChanged, int, CurrentValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FColorAmmoChanged, const FLinearColor, ColorParam);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPauseGame);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FResumeGame);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FQuitGame);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAmmoPickedUp);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDashAmountIncrement);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDashAmountDecrement);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPotionsAmountIncrement);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPotionsAmountDecrement);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBoltsAmountIncrement);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBoltsAmountDecrement);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FLookedAtInteractable, bool, IsInteractable, FString, InteractText);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUnlookedAtInteractable);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_SevenParams(FLookedAtModule, const FOPAG_GunStats, CurrentModule, 
const FOPAG_GunStats, OtherModule, const FString, OtherModuleName, const EModuleRarity, OtherModuleRarity, const EModuleTypes, OtherModuleType,
const int, EquippableTypes, const bool, CanBeEquipped);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUnlookedAtModule);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FLookedAtGun, const FOPAG_GunStats, GunStats, 
const FOPAG_GunStats, OtherGunStats, const FString, OtherGunName, const FString, OtherGunFireType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUnlookedAtGun);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGoldChanged, int, CurrentValue);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEnemyKilled, const AOPAG_Enemy* const, DeadEnemy);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDamagedBy, AActor* const, Causer);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMoveForward, const FInputActionValue&, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMoveRight, const FInputActionValue&, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLookUp, const FInputActionValue&, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTurn, const FInputActionValue&, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FJumping);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStopJumping);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSwapGuns);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FReloadGun);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStartShoot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKeepShoot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStopShoot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInteractWithItem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSwitchToADSCamera);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FResetFPSCamera);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUsePotion);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FActiveDash);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FActiveGunAbility);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInspectGun);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FCastAbility, EAbilityTypes, AbilityType, AActor*, Caller, AActor*, Target, EAbilitySpawnPattern, AbilitySpawnPattern);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBrokenForceField);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDestroyedPillar);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDisableGunTextInVendor, AOPAG_ModularGun*, Gun);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDisableModuleTextInVendor, AOPAG_ModuleBase*, Module);

UCLASS()
class OFPLANETSANDGUNS_API UOPAG_EventManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	FVector C;

	UPROPERTY(BlueprintAssignable, Category = Game)
		FStartPlanet StartPlanet;
	UPROPERTY(BlueprintAssignable, Category = Game)
		FLoadMainMenu LoadMainMenu;

	UPROPERTY(BlueprintAssignable, Category = StageManager)
		FStageManagerInit StageManagerInit;
	UPROPERTY(BlueprintAssignable, Category = StageManager)
		FStageManagerReady StageManagerReady;
	UPROPERTY(BlueprintAssignable, Category = StageManager)
		FGenerateStage GenerateStage;
	UPROPERTY(BlueprintAssignable, Category = StageManager)
		FRoomSpawned RoomSpawned;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = StageManager)
		FStageLoaded StageLoaded;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = StageManager)
		FPlacePlayerAtStart PlacePlayerAtStart;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = StageManager)
		FStageDifficultyUpdated StageDifficultyUpdated;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = StageManager)
		FStageCleared StageCleared;

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
		FRequestSavePlayerState RequestSavePlayerState;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = Player)
		FRequestPlayerState RequestPlayerState;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = Player)
		FSavePlayerState SavePlayerState;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = Player)
		FApplyPlayerState ApplyPlayerState;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = Player)
		FPlayerDied PlayerDied;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = Player)
		FHealthChanged HealthChanged;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = Player)
		FShieldChanged ShieldChanged;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = Player)
		FAmmoChanged TotalAmmoChanged;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = Player)
		FAmmoChanged CurrentAmmoChanged;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = Player)
		FColorAmmoChanged ColorAmmoChanged;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = Player)
		FGoldChanged GoldChanged;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = Player)
		FDashAmountIncrement DashAmountIncrement;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = Player)
		FDashAmountDecrement DashAmountDecrement;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = Player)
		FPotionsAmountIncrement PotionsAmountIncrement;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = Player)
		FPotionsAmountDecrement PotionsAmountDecrement;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = Player)
		FBoltsAmountIncrement BoltsAmountIncrement;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = Player)
		FBoltsAmountDecrement BoltsAmountDecrement;

	//Gun stuff
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = Gun)
		FAmmoPickedUp AmmoPickedUp;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = Gun)
		FLookedAtModule LookedAtModule;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = Gun)
		FUnlookedAtModule UnlookedAtModule;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = Gun)
		FLookedAtGun LookedAtGun;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = Gun)
		FUnlookedAtGun UnlookedAtGun;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = Gun)
		FLookedAtInteractable LookedAtInteractable;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = Gun)
		FUnlookedAtInteractable UnlookedAtInteractable;

    //Enemy stuff
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = EnemyManager)
		FEnemyKilled EnemyKilled;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = Enemy)
		FDamagedBy DamagedBy;

	//Game stuff
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = Game)
		FPauseGame PauseGame;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = Game)
		FResumeGame ResumeGame;
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
		FStartShoot StartShoot;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Enhanced Player")
		FKeepShoot KeepShoot;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Enhanced Player")
		FStopShoot StopShoot;

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
		FUsePotion UsePotion;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Enhanced Player")
		FActiveDash ActiveDash;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Enhanced Player")
		FActiveGunAbility ActiveGunAbility;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Force Field Ability")
		FDestroyedPillar DestroyedPillar;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Force Field Ability")
		FBrokenForceField BrokenForceField;
	
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Ability")
		FCastAbility CastAbility;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Enhanced Player")
	FInspectGun InspectGun;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Vendor")
	FDisableGunTextInVendor DisableGunTextInVendor;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Vendor")
	FDisableModuleTextInVendor DisableModuleTextInVendor;
};
