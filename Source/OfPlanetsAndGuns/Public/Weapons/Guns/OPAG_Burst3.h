#pragma once

#include "CoreMinimal.h"
#include "Weapons/Guns/OPAG_ModularGun.h"
#include "OPAG_Burst3.generated.h"

UCLASS()
class OFPLANETSANDGUNS_API AOPAG_Burst3 : public AOPAG_ModularGun
{
	GENERATED_BODY()
	
public:
	AOPAG_Burst3();

	UPROPERTY(Category = "Burst", VisibleAnywhere, BlueprintReadWrite)
	int BurstProjectileCount = 3;

	UPROPERTY(Category = "Burst", EditAnywhere, BlueprintReadWrite)
	int BurstProjectileCountMax = 3;

	UPROPERTY(Category = "Burst", EditAnywhere, BlueprintReadWrite)
	float TimeForAnyNextProjectile = .5f;

protected:
	FTimerHandle ShootBurstHandle;

public:
	// Main system for shooting
	virtual void Shoot(FVector StartShootLocation, FVector Direction, const AOPAG_CharacterBase* ShooterCharacter) override;

protected:
	void ShootBurst();
};
