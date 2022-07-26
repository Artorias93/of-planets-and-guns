#pragma once

#include "UObject/ConstructorHelpers.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OPAG_GunBase.generated.h"

class USkeletalMeshComponent;
class UChildActorComponent;
class UCameraComponent;

class AOPAG_ModuleBase;
enum class EModuleTypes : uint8;
enum class EModuleRarity : uint8;

class AOPAG_BarrelBase;
class AOPAG_AccessoryBase;
class AOPAG_SightBase;

class AOPAG_CharacterBase;
class AOPAG_Grenade;

class UKismetSystemLibrary;

UENUM(BlueprintType, meta = (Bitflags))
enum class EFireTypes : uint8
{
	FullyAutomatic = 0x00,
	SingleShot = 0x01,
	Burst3 = 0x02,
	Shotgun = 0x03,
};
ENUM_CLASS_FLAGS(EFireTypes);

USTRUCT(BlueprintType)
struct FGunStats
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FireRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Accuracy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RecoilRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ReloadingTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ClipSize;

	FGunStats() : Damage(0), FireRate(0), Accuracy(0), RecoilRate(0), ReloadingTime(0), ClipSize(0) {}
};

UCLASS()
class OFPLANETSANDGUNS_API AOPAG_GunBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AOPAG_GunBase();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Receiver;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* ADSCamera;

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
	UPROPERTY(Category = "Gun", VisibleAnywhere, BlueprintReadWrite)
	FName Name;

	UPROPERTY(Category = "Gun", EditDefaultsOnly, BlueprintReadWrite)
	EFireTypes Type;

	UPROPERTY(Category = "Gun", EditAnywhere, BlueprintReadWrite)
	FGunStats Stats;

	UPROPERTY(Category = "Gun", EditAnywhere, BlueprintReadWrite)
	int AmmoCurrent = 0;

	UPROPERTY(Category = "Gun", EditAnywhere, BlueprintReadWrite)
	int AmmoMax = 0;

	UPROPERTY(Category = "Gun", VisibleAnywhere, BlueprintReadWrite)
	bool bCanShoot = true;

	UPROPERTY(Category = "Gun", VisibleAnywhere, BlueprintReadWrite)
	bool bIsReloading = false;

	UPROPERTY(Category = "Gun", VisibleAnywhere, BlueprintReadWrite)
	bool bIsShoot = false;

	UPROPERTY(Category = "Gun", EditAnywhere, BlueprintReadWrite)
	float SpreadMultiplier = 1.f;

	UPROPERTY(Category = "Gun", EditAnywhere, BlueprintReadWrite)
	float SpreadMax = 12.f;

	UPROPERTY(Category = "Gun", EditAnywhere, BlueprintReadWrite)
	float ProjectileRange = 2000.f;

	UPROPERTY(Category = "Gun", EditAnywhere, BlueprintReadWrite)
	float ShootConeLenght = 100.f;

	UPROPERTY(Category = "Gun", EditAnywhere, BlueprintReadWrite)
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesQuery;

private:
	TSubclassOf<AOPAG_ModuleBase> StandardBarrel;
	TSubclassOf<AOPAG_ModuleBase> StandardStock;
	TSubclassOf<AOPAG_ModuleBase> StandardGrip;
	TSubclassOf<AOPAG_ModuleBase> StandardMagazine;

private:
	FTimerHandle ResetShootHandle;
	FTimerHandle ReloadHandle;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	// Mathematic Utility
	const float CalculateRateInSeconds(const float FireRate);

	// Main system for shooting
	void Shoot(FVector StartShootLocation, FVector Direction, const AOPAG_CharacterBase* ShooterActor);

	// Reload system
	void Reload();

	// Camera behavior
	UCameraComponent* GetSightCamera();
	UCameraComponent* GetADSCamera();

	// Module
	const bool AddNewModule(const TSubclassOf<AOPAG_ModuleBase> NewModule, const EModuleRarity NewModuleRarity, const FGunStats NewModuleStats, const int NewModuleEquippableFireTypes, TSubclassOf<AOPAG_ModuleBase>& OldModuleClass, EModuleRarity& OldModuleRarity, FGunStats& OldModuleStats, int& OldModuleEquippableFireTypes);
private:
	// Utility for link modules and sockets
	AOPAG_ModuleBase* GetModule(UChildActorComponent* ChildeComponent) const; 
	AOPAG_ModuleBase* GetModuleBySocketType(const EModuleTypes ModuleType) const;
	UChildActorComponent* GetSocketByModuleType(const EModuleTypes ModuleType) const;
	const FName GetSocketNameByModuleType(const EModuleTypes ModuleType) const;

	// Main system for shooting
	void ShootLineTrace(FVector StartShootLocation, FVector Direction, const AOPAG_CharacterBase* ShooterActor);
	void ShootThrowable(const FRotator SpawnRotation);
	void SpawnThrowable(const FVector SpawnLocation, const FRotator SpawnRotation);
	void ResetShootTimer();

	// Subsystem for shooting
	const float CalculateSpread();
	UFUNCTION(BlueprintCallable) const float CalculateSpreadRadius();
	const FVector2D CalculateRandPointInCircle();
	const FVector ApplyNoise(const FVector Direction);
	const FVector ApplyWeaponRange(const FVector StartLocation, const FVector Direction);
	TArray<AActor*> CheckWhatActorsIgnoreLinetrace();
	
	// Ammo behavior
	void UpdateAmmo();
	void WaitReloadingTimer();

	// Create and Attach module behavior
	void CreateAndAttachAllModules();
	void CreateModule(UChildActorComponent* ModuleSlot, const TSubclassOf<AOPAG_ModuleBase> Module);
	const bool AttachModule(UChildActorComponent* ModuleSlot, const TSubclassOf<AOPAG_ModuleBase> NewModule, const EModuleRarity NewModuleRarity, const FGunStats NewModuleStats, const int NewModuleEquippableFireTypes);
};