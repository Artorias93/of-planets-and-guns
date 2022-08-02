#pragma once

#include "UObject/ConstructorHelpers.h"
#include <Engine/DataTable.h>

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OPAG_GunStats.h"
#include "OPAG_GunBase.generated.h"

class USkeletalMeshComponent;
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
	FOPAG_GunStats Stats;

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

public:
	UPROPERTY(Category = "Emitter", EditDefaultsOnly, BlueprintReadWrite)
	UParticleSystem* MuzzlePartile;

	UPROPERTY(Category = "Emitter", EditDefaultsOnly, BlueprintReadWrite)
	UParticleSystem* HitPartile;

	UPROPERTY(Category = "Sound", EditDefaultsOnly, BlueprintReadWrite)
	USoundBase* ShootSound;

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

	// Subsystem shooting
	UFUNCTION(BlueprintCallable) const float CalculateSpreadRadius();

	// Camera behavior
	UCameraComponent* GetADSCamera();
protected:
	// Main system for shooting
	virtual void ShootLineTrace(FVector StartShootLocation, FVector Direction, const AOPAG_CharacterBase* ShooterCharacter);
	virtual USceneComponent* GetMuzzle() const;
	void ResetShootTimer();

	// Subsystem for shooting
	const float CalculateSpread();
	const FVector2D CalculateRandPointInCircle();
	const FVector ApplyNoise(const FVector Direction);
	const FVector ApplyWeaponRange(const FVector StartLocation, const FVector Direction);
	virtual TArray<AActor*> CheckWhatActorsIgnoreLinetrace();
};