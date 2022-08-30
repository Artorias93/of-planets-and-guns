#pragma once

#include "CoreMinimal.h"
#include "OPAG_GunState.h"
#include "Weapons/Guns/OPAG_GunBase.h"
#include "Interactables/OPAG_InteractableInterface.h"
#include "OPAG_ModularGun.generated.h"

class AOPAG_ModuleBase;
enum class EModuleTypes : uint8;
enum class EModuleRarity : uint8;

class AOPAG_BarrelBase;
class AOPAG_AccessoryBase;
class AOPAG_SightBase;

class AOPAG_CharacterBase;
class AOPAG_Grenade;

class UOPAG_EventManagerSubsystem;

class UKismetSystemLibrary;
class UDataTable;

class UGameplayAbility;

UCLASS()
class OFPLANETSANDGUNS_API AOPAG_ModularGun : public AOPAG_GunBase, public IOPAG_InteractableInterface
{
	GENERATED_BODY()

public:
	AOPAG_ModularGun();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UChildActorComponent* Barrel;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UChildActorComponent* Stock;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UChildActorComponent* Grip;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UChildActorComponent* Magazine;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UChildActorComponent* Sight;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UChildActorComponent* Accessory;

protected:
	UPROPERTY(Category = "OPAG | Gun | Settings", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float PriceBase = .0f;	

	UPROPERTY(Category = "OPAG | Gun | Ammo", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    UDataTable* AmmoReloadingMultipliersTable;

protected:
	UPROPERTY(Category = "OPAG | Gun | Standard Modules", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AOPAG_ModuleBase> StandardBarrel;

	UPROPERTY(Category = "OPAG | Gun | Standard Modules", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AOPAG_ModuleBase> StandardStock;

	UPROPERTY(Category = "OPAG | Gun | Standard Modules", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AOPAG_ModuleBase> StandardGrip;

	UPROPERTY(Category = "OPAG | Gun | Standard Modules", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AOPAG_ModuleBase> StandardMagazine;

	UPROPERTY(Category = "OPAG | Gun | Standard Modules", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AOPAG_ModuleBase> StandardSight;

protected:
	FTimerHandle FlyRotationHandle;
	TArray<FOPAG_ReloadStats*> AmmoReloadingMultipliers;
	FOPAG_GunStats StatsTotal;

protected:
	virtual void BeginPlay() override;

	// Muzzle with different getter method
	virtual USceneComponent* GetMuzzle() const override;

	// Update Stats
	void UpdateStatsTotal();

	// Create and Attach module behavior
	void CreateModule(UChildActorComponent* ModuleSlot, const TSubclassOf<AOPAG_ModuleBase> Module) const;
	bool AttachModule(UChildActorComponent* ModuleSlot, const TSubclassOf<AOPAG_ModuleBase> NewModule, const EModuleRarity NewModuleRarity, const FOPAG_GunStats NewModuleStats, const int NewModuleEquippableFireTypes) const;

	// Ammo behavior
	void UpdateAmmoWhenPickTheMagazine();
	virtual void WaitReloadingTimer() override;
	void PopulateAmmoTable();

	// Physics
	void CheckIfPhysical() const;

public:
	virtual void Tick(float DeltaTime) override;

	// Interface implements
	void ApplyForce_Implementation(const FVector Force) override;
	void ApplyFly_Implementation() override;
	void FlyRotationTimer_Implementation() override;
	bool CanBuy_Implementation() override;
	const float GetPriceTotal_Implementation() override;	

	// Increment ammo system
	void IncrementAmmo();

	// Module
	void CreateAndAttachAllModules(const FOPAG_GunModules& Modules);
	bool AddNewModule(const TSubclassOf<AOPAG_ModuleBase> NewModule, const EModuleRarity NewModuleRarity, const FOPAG_GunStats NewModuleStats, const int NewModuleEquippableFireTypes, TSubclassOf<AOPAG_ModuleBase>& OldModuleClass, EModuleRarity& OldModuleRarity, FOPAG_GunStats& OldModuleStats, int& OldModuleEquippableFireTypes);

	void GetCurrentState(FOPAG_GunState& GunState) const;

	// Shooting with throwable
	UFUNCTION(BlueprintCallable) void ShootThrowable(const FRotator SpawnRotation);

	// Interactable methods
	virtual void Interact_Implementation() override;
	virtual bool CanInteract_Implementation() override;
	FString InteractString_Implementation() override;

	// Detach all modules
	void DetachAllModules(const FVector Force);
	void DetachModule(UChildActorComponent* ChildModule, const FVector Force);

protected:
	// Shoot behavior
	UFUNCTION(BlueprintCallable) void SpawnThrowable(const FVector SpawnLocation, const FRotator SpawnRotation);

public:
	// Checks
	bool HasAccessory() const;
	bool HasSight() const;

public:
	// Generic getter
	static FString GetFireTypeName(EFireTypes FireType);
	virtual FOPAG_GunStats GetStatsTotal() const override;

	// Ability system getter
	TSubclassOf<UGameplayAbility> GetAbility() const;

	// Modules struct getter
	FOPAG_GunModules GetModulesStruct() const;
	FOPAG_GunModules GetStandardModulesStruct() const;

	// Modules getter
	AOPAG_ModuleBase* GetModule(const UChildActorComponent* const ChildComponent) const;
	UChildActorComponent* GetSocketByModuleType(const EModuleTypes ModuleType) const;
	AOPAG_ModuleBase* GetModuleBySocketType(const EModuleTypes ModuleType) const;
	FName GetSocketNameByModuleType(const EModuleTypes ModuleType) const;

	// Camera sight getter
	UCameraComponent* GetSightCamera() const;

	//Setter
	virtual void SetStatsTotal(const FOPAG_GunStats GunStats);
	void SetAmmoParams(int NewAmmoCurrent, int NewAmmoMax);
};
