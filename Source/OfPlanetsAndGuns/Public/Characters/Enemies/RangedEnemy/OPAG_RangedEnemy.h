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

	bool GetHasShotSinceSpawning() const;

	void SetHasShotSinceSpawning(const bool Value);

	UChildActorComponent* GetWeapon() const;

	UStaticMeshComponent* GetAimingLed() const;

	UNiagaraComponent* GetLaserBeam() const;

	virtual void GetShootingDirection(FVector& Start, FVector& ShootDirection, float const ShootingAccuracyTolerance /* = 0 */) override;

private:

	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaSeconds) override;
	
	void StartShooting();

	/////////////////////////////
	//Variables
	/////////////////////////////
	
private:

	UPROPERTY(VisibleAnywhere)
	UChildActorComponent* Weapon;
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* AimingLed;

	UPROPERTY(VisibleAnywhere)
	UNiagaraComponent* LaserBeam;
	
	FTimerHandle ShotTimerHandle;

	UPROPERTY(EditAnywhere, Category = "OPAG AI", meta=(AllowPrivateAccess = true))
	float ShotWaitTimeRandomIntervalBounds[2] = {0.4f, 0.6f};

	UPROPERTY(EditAnywhere, Category = "OPAG AI", meta = (AllowPrivateAccess = true))
	float RandomAngleShotBounds = 2.5f;



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

	//Used in BTService_Shoot to check if the laser beam has to be rendered
	bool bHasShotSinceSpawning = false;
};