#include "Weapons/Guns/OPAG_Pistol.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Characters/OPAG_CharacterBase.h"
#include <Kismet/GameplayStatics.h>
#include <Engine/StaticMeshActor.h>
#include "Characters/Enemies/OPAG_Enemy.h"

AOPAG_Pistol::AOPAG_Pistol()
{
	MuzzleComponent = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleComponent"));
	MuzzleComponent->SetupAttachment(RootComponent);
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
	const FVector NoiseDirection = ApplyNoise(Direction);
	const FVector EffectiveDirection = ApplyWeaponRange(StartShootLocation, NoiseDirection);

	// Initialize out of hit and if check is true (hit object in scene) so the actor is cast to character and control the return value
	FHitResult OutHit;
	const TArray<AActor*> ActorsToIgnore = CheckWhatActorsIgnoreLinetrace();
	const bool bIsHit = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), StartShootLocation, EffectiveDirection, ObjectTypesQuery, false,
		ActorsToIgnore, EDrawDebugTrace::Persistent, OutHit, true, FLinearColor::Red, FLinearColor::Green, 5.f);

	// If Actor is inherited by character base, invoke take damage
	if (bIsHit)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitPartile, OutHit.Location, FRotator::ZeroRotator, true, EPSCPoolMethod::None, true);

		AOPAG_CharacterBase* Character = Cast<AOPAG_CharacterBase>(OutHit.GetActor());
		if (Character)
		{
			//Checks for friendly fire
			//If both the shooter and the target are enemies, damage is not applied
			const AOPAG_Enemy* const EnemyShooter = Cast<AOPAG_Enemy>(ShooterCharacter);
			const AOPAG_Enemy* const EnemyShot = Cast<AOPAG_Enemy>(Character);
			if(!(EnemyShooter && EnemyShot))
			{
				//Check for shield enemy
				//If the hit component is the shield, damage is not applied
				if(!OutHit.GetComponent()->ComponentHasTag("Shield"))
				{
					Character->ReceiveDamage(Stats.Damage);
				}
			}			
		}
	}

	// Spawn emitter
	// const FVector MuzzleLocation = GetMuzzle()->GetActorLocation();
	UGameplayStatics::SpawnEmitterAttached(MuzzlePartile, GetMuzzle(), NAME_None, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true, EPSCPoolMethod::None, true);
}

USceneComponent* AOPAG_Pistol::GetMuzzle() const
{
	if(MuzzleComponent)
		return MuzzleComponent;

	return nullptr;
}
#pragma endregion Shooting Behavior