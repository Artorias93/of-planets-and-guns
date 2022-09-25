#include "Weapons/Guns/OPAG_Shotgun.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Characters/OPAG_CharacterBase.h"
#include <Kismet/GameplayStatics.h>
#include "Characters/Enemies/OPAG_Enemy.h"
#include "Weapons/Guns/OPAG_GunBase.h"
#include "Managers/OPAG_GameInstance.h"
#include "Managers/OPAG_EventManagerSubsystem.h"
#include "Items/OPAG_Barrel.h"
#include "Abilities/OPAG_ForceField.h"
#include <Kismet/KismetMathLibrary.h>

AOPAG_Shotgun::AOPAG_Shotgun()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AOPAG_Shotgun::ShootLineTrace(FVector StartShootLocation, FVector Direction, const AOPAG_CharacterBase* ShooterCharacter)
{
	for (int i = 0; i < ConsecutiveProjectile; i++)
	{
		//TODO: Recoil Event
		const FVector NoiseDirection = ApplyNoise(Direction);
		const FVector EffectiveDirection = ApplyWeaponRange(StartShootLocation, NoiseDirection);

		// Initialize out of hit and if check is true (hit object in scene) so the actor is cast to character and control the return value
		FHitResult OutHit;
		const TArray<AActor*> ActorsToIgnore = CheckWhatActorsIgnoreLinetrace();
		const bool bIsHit = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), StartShootLocation, EffectiveDirection, ObjectTypesQuery, false,
			ActorsToIgnore, EDrawDebugTrace::None, OutHit, true, FLinearColor::Red, FLinearColor::Green, 5.f);

		// Draw line trace when shoot	
		FVector Offset = EffectiveDirection.GetSafeNormal() * 200.f;
		FVector TraceStartLocation = StartShootLocation + Offset;

		const FTransform TracerTransform = FTransform(EffectiveDirection.Rotation(), TraceStartLocation, FVector(.3f, .3f, .3f));
		GetWorld()->SpawnActor(ProjectileTrace, &TracerTransform);

		// If Actor is inherited by character base, invoke take damage
		if (bIsHit)
		{
			// Spawn bullet concrete
			const FVector BulletConcreteLocation = OutHit.ImpactPoint;
			const FRotator BulletConcreteRotation = OutHit.ImpactNormal.Rotation();
			UGameplayStatics::SpawnDecalAtLocation(GetWorld(), BulletConcreteDecal, FVector(5.f, 5.f, 5.f), BulletConcreteLocation, BulletConcreteRotation, 15.f);

			// Spawn particle when impact to hit point
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitPartile, OutHit.Location, FRotator::ZeroRotator, true, EPSCPoolMethod::None, true);

			// It should be implemented by a "Hittable interface, were the gun just check if the hitted actor has this interface, if yes, send the an hit event with the hitted actor as param"
			if (AOPAG_ForceField* ForceField = Cast<AOPAG_ForceField>(OutHit.GetActor()))
			{
				EMS->ForceFieldHit.Broadcast(ForceField);
			}

			// All behavior with targets
			AOPAG_CharacterBase* Character = Cast<AOPAG_CharacterBase>(OutHit.GetActor());
			if (Character)
			{
				//Checks for friendly fire
				//If both the shooter and the target are enemies, damage is not applied
				const AOPAG_Enemy* const EnemyShooter = Cast<AOPAG_Enemy>(ShooterCharacter);
				const AOPAG_Enemy* const EnemyShot = Cast<AOPAG_Enemy>(Character);
				if (!(EnemyShooter && EnemyShot))
				{
					//Check for shield enemy
					//If the hit component is the shield, damage is not applied
					if (!OutHit.GetComponent()->ComponentHasTag("Shield"))
					{
						//Check for head shot
						const bool IsHeadshot = OutHit.GetComponent()->ComponentHasTag("Head");
						const float MultiplierByLevel = CalculateMultiplierByLevel();
						Character->ReceiveDamage(CalculateDamage(IsHeadshot, MultiplierByLevel) / ConsecutiveProjectile, GetParentActor());
					}
				}
			}
			AOPAG_Barrel* ObjectBarrel = Cast<AOPAG_Barrel>(OutHit.GetActor());
			if (ObjectBarrel != nullptr)
			{
				ObjectBarrel->Detonate();
			}
		}
	}
}