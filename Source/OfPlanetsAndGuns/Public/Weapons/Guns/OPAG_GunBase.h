#pragma once

#include "UObject/ConstructorHelpers.h"
#include <Engine/DataTable.h>

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OPAG_GunStats.h"
#include "OPAG_GunBase.generated.h"

class USkeletalMeshComponent;
class UStaticMeshComponent;
class UChildActorComponent;
class UCameraComponent;
class AOPAG_CharacterBase;
class UOPAG_EventManagerSubsystem;
class UParticleSystem;
class USoundBase;

UCLASS()
class OFPLANETSANDGUNS_API AOPAG_GunBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AOPAG_GunBase();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Receiver;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* ADSCamera;

protected:
	UPROPERTY(Category = "OPAG | Gun | Settings", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FName Name;

	UPROPERTY(Category = "OPAG | Gun | Settings", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EFireTypes FireType;

	UPROPERTY(Category = "OPAG | Gun | Settings", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FOPAG_GunStats StatsBase;

protected:

	UPROPERTY(Category = "OPAG | Gun | Ammo", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int AmmoCurrent = 0;

	UPROPERTY(Category = "OPAG | Gun | Ammo", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int AmmoMax = 0;

protected:
	UPROPERTY(Category = "OPAG | Gun | Damage", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", UIMin = 0.f, UIMax = 100.f))
	float DamageRandomnessPercentage = 20.f;

	UPROPERTY(Category = "OPAG | Gun | Damage", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float CritDamageMultiplier = 3.f;

	UPROPERTY(Category = "OPAG | Gun | Damage", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", UIMin = 0.f, UIMax = 100.f))
	float BaseCritChance = 5.f;

protected:
	UPROPERTY(Category = "OPAG | Gun | Shooting", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float SpreadMultiplier = 1.f;

	UPROPERTY(Category = "OPAG | Gun | Shooting", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float SpreadMax = 12.f;

	UPROPERTY(Category = "OPAG | Gun | Shooting", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float ProjectileRange = 2000.f;

	UPROPERTY(Category = "OPAG | Gun | Shooting", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float ShootConeLength = 100.f;

	UPROPERTY(Category = "OPAG | Gun | Shooting", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float ReductionSpreadRadius = 10;

	UPROPERTY(Category = "OPAG | Gun | Shooting", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float ReductionSpreadRadiusShootingMax = 10.f;

	UPROPERTY(Category = "OPAG | Gun | Shooting", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesQuery;

protected:
	UPROPERTY(Category = "OPAG | Gun | Emitter", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UParticleSystem* MuzzlePartile;

	UPROPERTY(Category = "OPAG | Gun | Emitter", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UParticleSystem* HitPartile;

protected:
	UPROPERTY(Category = "OPAG | Gun | Sound", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USoundBase* ShootSound;

	UPROPERTY(Category = "OPAG | Gun | Sound", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USoundBase* ReloadSound;

protected:
	UOPAG_EventManagerSubsystem* EMS;

protected:
	float ReductionSpreadRadiusShooting = 1.f;

	static constexpr float MinInternalAccuracy = 97.f;
	static constexpr float MaxInternalAccuracy = 100.f;

protected:
	FTimerHandle ResetShootHandle;
	FTimerHandle ReloadHandle;

protected:
	// Boolean checks
	bool bCanShoot = true;
	bool bIsReloading = false;
	bool bIsShoot = false;

	// Shoot math calculation
	static constexpr float SecondsInOneMinute = 60.f;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	// Maths Utility
	static float CalculatePeriod(const float FireRate);

	// Main system for shooting
	virtual int Shoot(const FVector StartShootLocation, const FVector Direction, const AOPAG_CharacterBase* ShooterCharacter);

	// Reload system
	virtual void Reload();

	// Subsystem shooting
	UFUNCTION(BlueprintCallable) float CalculateSpreadRadius() const;

	// Camera behavior
	UCameraComponent* GetADSCamera() const;
	void ResetReductionSpreadRadiusShooting();

	// Checking
	bool CanShoot() const;
	bool CanReloading() const;
	bool CanAim() const;

protected:
	// Main system for shooting
	virtual void ShootLineTrace(const FVector StartShootLocation, const FVector Direction, const AOPAG_CharacterBase* ShooterCharacter);
	virtual USceneComponent* GetMuzzle() const;
	void ResetShootTimer();

	// Reload
	virtual void WaitReloadingTimer();

	// Subsystem for shooting
	float CalculateDamage(const bool IsHeadshot) const;
	void AddCritToDamage(float& Damage) const;
	float CalculateSpread() const;
	FVector2D CalculateRandPointInCircle() const;
	FVector ApplyNoise(const FVector Direction) const;
	FVector ApplyWeaponRange(const FVector StartLocation, const FVector Direction) const;
	TArray<AActor*> CheckWhatActorsIgnoreLinetrace();

public:
	// Getter
	FName GetGunName() const;
	EFireTypes GetFireType() const;
	virtual FOPAG_GunStats GetStatsTotal() const;
	int GetAmmoCurrent() const;
	int GetAmmoMax() const;

	//Setter
	virtual void SetStatsTotal(const FOPAG_GunStats GunStats);
	void SetAmmoCurrent(const int Value);
	void SetAmmoMax(const int Value);
	void DecrementAmmoCurrent(const int Value);

public:
	FORCEINLINE USkeletalMeshComponent* GetReceiverMesh() { return Receiver; }

};
