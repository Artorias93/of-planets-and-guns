#pragma once

#include "UObject/ConstructorHelpers.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Engine/DataTable.h>
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

class UOPAG_EventManagerSubsystem;

class UKismetSystemLibrary;
class UDataTable;

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
struct OFPLANETSANDGUNS_API FOPAG_ReloadStats : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EFireTypes FireType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int ReloadingFactor;

};

USTRUCT(BlueprintType)
struct OFPLANETSANDGUNS_API FGunStats : public FTableRowBase
{
	GENERATED_BODY()

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

public:
	UPROPERTY(Category = "Gun", EditDefaultsOnly, BlueprintReadWrite)
	FName Name;

	UPROPERTY(Category = "Gun", EditDefaultsOnly, BlueprintReadWrite)
	EFireTypes FireType;

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

protected:
	UOPAG_EventManagerSubsystem* EMS;

protected:
	FTimerHandle ResetShootHandle;
	FTimerHandle ReloadHandle;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	// Mathematic Utility
	const float CalculateRateInSeconds(const float FireRate);

	// Main system for shooting
	virtual void Shoot(FVector StartShootLocation, FVector Direction, const AOPAG_CharacterBase* ShooterCharacter);

	// Camera behavior
	UCameraComponent* GetADSCamera();
protected:
	// Main system for shooting
	virtual void ShootLineTrace(FVector StartShootLocation, FVector Direction, const AOPAG_CharacterBase* ShooterCharacter);
	void ResetShootTimer();

	// Subsystem for shooting
	const float CalculateSpread();
	UFUNCTION(BlueprintCallable) const float CalculateSpreadRadius();
	const FVector2D CalculateRandPointInCircle();
	const FVector ApplyNoise(const FVector Direction);
	const FVector ApplyWeaponRange(const FVector StartLocation, const FVector Direction);
	virtual TArray<AActor*> CheckWhatActorsIgnoreLinetrace();
};