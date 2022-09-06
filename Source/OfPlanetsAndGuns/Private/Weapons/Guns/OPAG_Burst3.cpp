#include "Weapons/Guns/OPAG_Burst3.h"
#include "Characters/OPAG_CharacterBase.h"
#include "Managers/OPAG_SoundManagerSubsystem.h"
#include <Kismet/GameplayStatics.h>

AOPAG_Burst3::AOPAG_Burst3()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AOPAG_Burst3::BeginPlay()
{
	Super::BeginPlay();

	BurstProjectileCountMax = BurstProjectileCount;
}

int AOPAG_Burst3::Shoot(FVector StartShootLocation, FVector Direction, const AOPAG_CharacterBase* ShooterCharacter)
{
	ShootBurst();
	return BurstProjectileCountMax;
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

			ShootLineTrace(StartShootLocation, Direction, Character);

			ReductionSpreadRadiusShooting = FMath::Clamp(ReductionSpreadRadiusShooting + .15f, 1.f, ReductionSpreadRadiusShootingMax);

			//UGameplayStatics::PlaySound2D(GetWorld(), ShootSound);
			SMS->PlayWeaponSounds(this, EWeaponSoundAction::Fire);
			if (AOPAG_CharacterBase* const CharacterOwner = Cast<AOPAG_CharacterBase>(GetParentActor()))
			{
				MakeNoise(1.f, CharacterOwner, CharacterOwner->GetActorLocation(), 0);
			}

			// Update number of consecutive projectiles
			BurstProjectileCount--;

			// Wait time to next projectile
			GetWorld()->GetTimerManager().SetTimer(ShootBurstHandle, this, &AOPAG_Burst3::ShootBurst, TimeForAnyNextProjectile, false);
		}
	}
	else
	{
		BurstProjectileCount = BurstProjectileCountMax;
	}
}