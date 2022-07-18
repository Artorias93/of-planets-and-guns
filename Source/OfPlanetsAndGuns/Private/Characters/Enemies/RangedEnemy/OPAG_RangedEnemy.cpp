#include "Characters/Enemies/RangedEnemy/OPAG_RangedEnemy.h"
#include <Kismet/KismetMathLibrary.h>
//My Classes
#include "Weapons/Guns/OPAG_GunBase.h"
#include "Characters/Enemies/RangedEnemy/OPAG_AICRangedEnemy.h"

AOPAG_RangedEnemy::AOPAG_RangedEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	//Components creation
	Weapon = CreateDefaultSubobject<UChildActorComponent>("Weapon Child Actor Component");
	if(Weapon)
	{
		Weapon->AttachToComponent(GetMesh(),
			FAttachmentTransformRules::KeepRelativeTransform, "RightHandSocket");
		//TODO: Which class to put here???
		Weapon->SetChildActorClass(AOPAG_GunBase::StaticClass());
		
		AimingLed = CreateDefaultSubobject<UStaticMeshComponent>("Aiming Led Static Mesh Component");		
		if(AimingLed)
		{
			AimingLed->AttachToComponent(Weapon, FAttachmentTransformRules::KeepRelativeTransform);
		}
		
		LaserBeam = CreateDefaultSubobject<UNiagaraComponent>("Laser Beam Niagara Component");
		if(LaserBeam)
		{
			LaserBeam->AttachToComponent(Weapon, FAttachmentTransformRules::KeepRelativeTransform);
		}
	}
}

void AOPAG_RangedEnemy::Tick(float DeltaSeconds)
{	
	Super::Tick(DeltaSeconds);
	
	if (const AOPAG_AICRangedEnemy* const AIController = Cast<AOPAG_AICRangedEnemy>(GetController()))
	{
		//TODO: Replace with proper player class when ready
		if(const AOPAG_CharacterBase* const Player = Cast<AOPAG_CharacterBase>(AIController->GetFocusActor()))
		{
			//Points the laser beam directly to the player
			//TODO: Get back to this when player is implemented
			LaserBeam->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(LaserBeam->K2_GetComponentLocation(),
				Player->GetActorLocation()));

			//Scales the aiming led based on the distance from the player
			//TODO: Get back to this when player is implemented
			AimingLed->SetWorldScale3D((((GetActorLocation() - Player->GetActorLocation()).Size()) / -100.f)
				* FVector(.01f, .01f, .01f));
		}
	}
}

void AOPAG_RangedEnemy::FireBullet()
{
	if(AOPAG_GunBase* const EquippedWeapon = Cast<AOPAG_GunBase>(Weapon->GetChildActor()))
	{
		//TODO: Replace when gun is implemented in the project
		if(/*EquippedWeapon->CanShoot*/ true)
		{
			FVector StartLocation = FVector::ZeroVector;
			FVector Direction = FVector::ZeroVector;

			//The first 2 params are multiple "return values" passed by reference.
			GetShootingDirection(StartLocation, Direction, ShootAccuracyTolerance);

			//A random wait from the moment the enemy decides to shoot and when it actually shoots
			const float ShotWaitTime = UKismetMathLibrary::RandomFloatInRange(ShotWaitTimeRandomIntervalBounds[0],
				ShotWaitTimeRandomIntervalBounds[1]);

			GetWorld()->GetTimerManager().SetTimer(ShotTimerHandle, this, &AOPAG_RangedEnemy::StartShooting,
				ShotWaitTime);
		}
	}
}

void AOPAG_RangedEnemy::StartShooting()
{
	GetWorld()->GetTimerManager().ClearTimer(ShotTimerHandle);

	//TODO: Uncomment when Gun is implemented
	//EquippedWeapon.StartShooting(StartLocation, Direction);
}