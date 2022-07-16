#include "Characters/Enemies/RangedEnemy/OPAG_RangedEnemy.h"
#include <Kismet/KismetMathLibrary.h>
//My Classes
#include "Weapons/Guns/OPAG_GunBase.h"
#include "Characters/Enemies/RangedEnemy/OPAG_AICRangedEnemy.h"

AOPAG_RangedEnemy::AOPAG_RangedEnemy()
{
	Weapon = CreateDefaultSubobject<UChildActorComponent>("Weapon Child Actor Component");
	if(Weapon)
	{
		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "RightHandSocket");
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
			//TODO: This works like shit
			LaserBeam->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(LaserBeam->K2_GetComponentLocation(), Player->GetActorLocation()));

			//TODO: This also works like shit
			AimingLed->SetWorldScale3D((((GetActorLocation() - Player->GetActorLocation()).Size()) / -100.f) * FVector(.01f, .01f, .01f));
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

			GetShootingDirection(StartLocation, Direction, ShootAccuracyTolerance);

			const float ShotWaitTime = UKismetMathLibrary::RandomFloatInRange(ShotWaitTimeRandomIntervalBounds[0], ShotWaitTimeRandomIntervalBounds[1]);

			GetWorld()->GetTimerManager().SetTimer(ShotTimerHandle, this, &AOPAG_RangedEnemy::StartShooting, ShotWaitTime);
		}
	}
}

void AOPAG_RangedEnemy::StartShooting()
{
	GetWorld()->GetTimerManager().ClearTimer(ShotTimerHandle);

	//Uncomment when Gun is implemented
	//EquippedWeapon.StartShooting(StartLocation, Direction);
}