#pragma once

#include "CoreMinimal.h"
#include "Weapons/Guns/OPAG_ModularGun.h"
#include "OPAG_Burst3.generated.h"

UCLASS()
class OFPLANETSANDGUNS_API AOPAG_Burst3 final : public AOPAG_ModularGun
{
	GENERATED_BODY()
	
public:
	AOPAG_Burst3();

private:
	UPROPERTY(Category = "OPAG | Gun | Shooting", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int BurstProjectileCount = 3;

	UPROPERTY(Category = "OPAG | Gun | Shooting", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float TimeForAnyNextProjectile = .5f;

private:
	int BurstProjectileCountMax;
	
private:
	FTimerHandle ShootBurstHandle;

public:
	// Main system for shooting
	virtual int Shoot(FVector StartShootLocation, FVector Direction, const AOPAG_CharacterBase* ShooterCharacter) override;

protected:
	virtual void BeginPlay() override;

private:
	void ShootBurst();

};
