#include "Weapons/Guns/OPAG_Shotgun.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Characters/OPAG_CharacterBase.h"

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
		TArray<AActor*> ActorsToIgnore = CheckWhatActorsIgnoreLinetrace();
		const bool bIsHit = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), StartShootLocation, EffectiveDirection, ObjectTypesQuery, false,
			ActorsToIgnore, EDrawDebugTrace::Persistent, OutHit, true, FLinearColor::Red, FLinearColor::Green, 5.f);

		// If Actor is inheritance by character base, invoke take damage
		if (bIsHit)
		{
			AOPAG_CharacterBase* Character = Cast<AOPAG_CharacterBase>(OutHit.GetActor());
			if (Character)
			{
				const float Damage = Stats.Damage / ConsecutiveProjectile;
				Character->ReceiveDamage(Damage);
			}
		}
	}
	AmmoCurrent--;
}