#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemies/OPAG_Enemy.h"
#include <../../../FX/Niagara/Source/Niagara/Public/NiagaraComponent.h>
#include "OPAG_RangedEnemy.generated.h"

UCLASS()
class OFPLANETSANDGUNS_API AOPAG_RangedEnemy : public AOPAG_Enemy
{
	GENERATED_BODY()

	/////////////////////////////
	//Functions
	/////////////////////////////

public:

	AOPAG_RangedEnemy();

	void FireBullet();

private:

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

private:

	UPROPERTY(EditAnywhere, Category = "OPAG AI", meta=(AllowPrivateAccess = true))
	float ShootAccuracyTolerance = 0;

	UPROPERTY()
	FTimerHandle ShotTimerHandle;

	UPROPERTY(EditAnywhere, Category = "OPAG AI", meta=(AllowPrivateAccess = true))
	float ShotWaitTimeRandomIntervalBounds[2] = {0.5f, 0.7f};
};