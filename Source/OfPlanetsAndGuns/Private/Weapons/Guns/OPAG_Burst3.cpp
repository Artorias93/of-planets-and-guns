#include "Weapons/Guns/OPAG_Burst3.h"
#include "Characters/OPAG_CharacterBase.h"
#include <Kismet/GameplayStatics.h>

AOPAG_Burst3::AOPAG_Burst3()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AOPAG_Burst3::Shoot(FVector StartShootLocation, FVector Direction, const AOPAG_CharacterBase* ShooterCharacter)
{
	bCanShoot = false;

	ShootBurst();

	ReductionSpreadRadiusShooting = FMath::Clamp(ReductionSpreadRadiusShooting + .15f, 1.f, ReductionSpreadRadiusShootingMax);
}

void AOPAG_Burst3::ShootBurst()
{
	if (BurstProjectileCount > 0)
	{
		AOPAG_CharacterBase* Character = Cast<AOPAG_CharacterBase>(GetParentActor());
		if (Character)
		{
			// Calculate start location and direction for any shoot
			FVector StartShootLocation;
			FVector Direction;
			Character->GetShootingDirection(StartShootLocation, Direction);

			// Shoot single line trace
			ShootLineTrace(StartShootLocation, Direction, Character);

			// Decrement number of consecutive projectiles
			BurstProjectileCount--;

			// Play sound
			UGameplayStatics::PlaySound2D(GetWorld(), ShootSound);

			// Re-call this method
			GetWorld()->GetTimerManager().SetTimer(ShootBurstHandle, this, &AOPAG_Burst3::ShootBurst, TimeForAnyNextProjectile, false);
		}
	}
	else
	{
		BurstProjectileCount = BurstProjectileCountMax;
		const float Seconds = CalculateRateInSeconds(Stats.FireRate);
		GetWorld()->GetTimerManager().SetTimer(ResetShootHandle, this, &AOPAG_Burst3::ResetShootTimer, Seconds, false);
	}
}