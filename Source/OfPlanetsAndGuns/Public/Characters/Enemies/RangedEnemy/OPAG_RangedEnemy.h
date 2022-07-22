#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemies/OPAG_Enemy.h"
#include <../../../FX/Niagara/Source/Niagara/Public/NiagaraComponent.h>
#include "OPAG_RangedEnemy.generated.h"

class AOPAG_GunBase;

UCLASS()
class OFPLANETSANDGUNS_API AOPAG_RangedEnemy : public AOPAG_Enemy
{
	GENERATED_BODY()

	/////////////////////////////
	//Functions
	/////////////////////////////

public:

	AOPAG_RangedEnemy();

	virtual void Attack() override;

	virtual void Death() override;

private:

	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaSeconds) override;
	
	void StartShooting();

	/////////////////////////////
	//Variables
	/////////////////////////////

public:

	UPROPERTY(VisibleAnywhere)
	UChildActorComponent* Weapon;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* AimingLed;

	UPROPERTY(VisibleAnywhere)
	UNiagaraComponent* LaserBeam;

	bool bIsShooting = false;

private:

	UPROPERTY(EditAnywhere, Category = "OPAG AI", meta=(AllowPrivateAccess = true))
	float ShootAccuracyTolerance = 0;
	
	FTimerHandle ShotTimerHandle;

	UPROPERTY(EditAnywhere, Category = "OPAG AI", meta=(AllowPrivateAccess = true))
	float ShotWaitTimeRandomIntervalBounds[2] = {0.4f, 0.6f};

	UPROPERTY()
	AOPAG_GunBase* EquippedWeapon;

	//Vectors for LaserBeam
	UPROPERTY()
	FVector StartLocation = FVector::ZeroVector;
	UPROPERTY()
	FVector Direction = FVector::ZeroVector;

	//Modifies the Laser Beam direction 
	UPROPERTY(EditAnywhere, Category = "OPAG AI", meta=(AllowPrivateAccess=true))
	FVector LaserBeamDestinationModifier = FVector::ZeroVector;
};