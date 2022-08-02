#include "Weapons/Guns/OPAG_Pistol.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Characters/OPAG_CharacterBase.h"

AOPAG_Pistol::AOPAG_Pistol()
{

}

void AOPAG_Pistol::BeginPlay()
{
	Super::BeginPlay();
}

void AOPAG_Pistol::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

#pragma region Shooting Behavior
void AOPAG_Pistol::ShootLineTrace(FVector StartShootLocation, FVector Direction, const AOPAG_CharacterBase* ShooterCharacter)
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
			Character->ReceiveDamage(Stats.Damage);
		}
	}
}
#pragma endregion Shooting Behavior