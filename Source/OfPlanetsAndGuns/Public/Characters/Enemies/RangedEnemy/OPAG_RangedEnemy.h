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

	void Tick(float DeltaSeconds) override;

	void FireBullet();

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

	UPROPERTY(EditAnywhere, Category = "OPAG AI")
	float ShootAccuracyTolerance = 0;

private:

	FTimerHandle ShotTimerHandle;	

	UPROPERTY(EditAnywhere, Category = "OPAG AI")
	float ShotWaitTimeRandomIntervalBounds[2] = {0.5f, 0.7f};
};