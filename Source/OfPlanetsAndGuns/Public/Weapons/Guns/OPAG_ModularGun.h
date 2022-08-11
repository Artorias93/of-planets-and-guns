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

UCLASS()
class OFPLANETSANDGUNS_API AOPAG_ModularGun : public AOPAG_GunBase, public IOPAG_InteractableInterface
{
	GENERATED_BODY()

public:
	AOPAG_ModularGun();

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

public:
	UPROPERTY(Category = "Gun", EditAnywhere, BlueprintReadWrite)
	FOPAG_GunStats BaseStats;

	UPROPERTY(Category = "Gun", EditAnywhere, BlueprintReadWrite)
    UDataTable* AmmoReloadingMultipliersTable;

	UPROPERTY(Category = "Sound", EditDefaultsOnly, BlueprintReadWrite)
	USoundBase* ReloadSound;

public:
	UPROPERTY(Category = StandardModules, EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AOPAG_ModuleBase> StandardBarrel;

	UPROPERTY(Category = StandardModules, EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AOPAG_ModuleBase> StandardStock;

	UPROPERTY(Category = StandardModules, EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AOPAG_ModuleBase> StandardGrip;

	UPROPERTY(Category = StandardModules, EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AOPAG_ModuleBase> StandardMagazine;

private:
	TArray<FOPAG_ReloadStats*> AmmoReloadingMultipliers;

protected:
	virtual void BeginPlay() override;
	virtual USceneComponent* GetMuzzle() const override;

public:
	virtual void Tick(float DeltaTime) override;
	void ApplyForce_Implementation(const FVector Force) override;

	// Setter and Getter
	void SetAmmoParams(int NewAmmoCurrent, int NewAmmoMax);
	FOPAG_GunModules GetModulesStruct() const;

	// Utility for link modules and sockets
	AOPAG_ModuleBase* GetModule(const UChildActorComponent* const ChildComponent) const;
	UChildActorComponent* GetSocketByModuleType(const EModuleTypes ModuleType) const;

	// Reload system
	void Reload();

	// Increment ammo system
	void IncrementAmmo();

	// Module
	void CreateAndAttachAllModules(const FOPAG_GunModules& Modules);
	bool AddNewModule(const TSubclassOf<AOPAG_ModuleBase> NewModule, const EModuleRarity NewModuleRarity, const FOPAG_GunStats NewModuleStats, const int NewModuleEquippableFireTypes, TSubclassOf<AOPAG_ModuleBase>& OldModuleClass, EModuleRarity& OldModuleRarity, FOPAG_GunStats& OldModuleStats, int& OldModuleEquippableFireTypes);

	// Camera behavior
	UCameraComponent* GetSightCamera() const;

	void GetCurrentState(FOPAG_GunState& GunState) const;

	UFUNCTION(BlueprintCallable) void ShootThrowable(const FRotator SpawnRotation);

	AOPAG_ModuleBase* GetModuleBySocketType(const EModuleTypes ModuleType) const;
	FName GetSocketNameByModuleType(const EModuleTypes ModuleType) const;

	virtual void Interact_Implementation() override;

	virtual bool CanInteract_Implementation() override;

protected:
	// Shoot behavior
	UFUNCTION(BlueprintCallable) void SpawnThrowable(const FVector SpawnLocation, const FRotator SpawnRotation);

	// Create and Attach module behavior
	void CreateModule(UChildActorComponent* ModuleSlot, const TSubclassOf<AOPAG_ModuleBase> Module) const;
	bool AttachModule(UChildActorComponent* ModuleSlot, const TSubclassOf<AOPAG_ModuleBase> NewModule,
		const EModuleRarity NewModuleRarity, const FOPAG_GunStats NewModuleStats, const int NewModuleEquippableFireTypes) const;
	void UpdateStats();

	// Ammo behavior
	void UpdateAmmo();
	void WaitReloadingTimer();
	void PopulateAmmoTable();

	void CheckIfPhysical() const;
};
