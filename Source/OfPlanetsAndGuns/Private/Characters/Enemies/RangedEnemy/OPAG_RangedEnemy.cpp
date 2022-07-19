#include "Characters/Enemies/RangedEnemy/OPAG_RangedEnemy.h"
#include <Kismet/KismetMathLibrary.h>
//My Classes
#include "Weapons/Guns/OPAG_GunBase.h"
#include "Characters/Enemies/RangedEnemy/OPAG_AICRangedEnemy.h"
#include "Characters/Player/OPAG_Player.h"

AOPAG_RangedEnemy::AOPAG_RangedEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	//Components creation
	Weapon = CreateDefaultSubobject<UChildActorComponent>("Weapon Child Actor Component");
	if(Weapon)
	{
		Weapon->SetupAttachment(RootComponent);
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

void AOPAG_RangedEnemy::BeginPlay()
{
	Super::BeginPlay();

	//Attaches gun to the mesh
	if(Weapon)
	{
		if(Weapon->GetChildActor())
		{
			Weapon->AttachToComponent(GetMesh(),
				FAttachmentTransformRules::SnapToTargetNotIncludingScale, "RightHandSocketGun");
			EquippedWeapon = Cast<AOPAG_GunBase>(Weapon->GetChildActor());
		}
	}	
}

void AOPAG_RangedEnemy::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (const AOPAG_AICRangedEnemy* const AIController = Cast<AOPAG_AICRangedEnemy>(GetController()))
	{
		if(const AOPAG_Player* const Player = Cast<AOPAG_Player>(AIController->GetFocusActor()))
		{
			//TODO: Add a PingPong function to this so it is more realistic?
			//Points the laser beam directly to the player
			LaserBeam->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(LaserBeam->K2_GetComponentLocation(),
				Player->GetActorLocation() + LaserBeamDestinationModifier));

			//Scales the aiming led based on the distance from the player
			AimingLed->SetWorldScale3D((((GetActorLocation() - Player->GetActorLocation()).Size()) / -100.f)
				* FVector(.01f, .01f, .01f));
		}
	}
}

void AOPAG_RangedEnemy::Attack()
{
	if(EquippedWeapon)
	{
		if(EquippedWeapon->bCanShoot)
		{
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
	
	EquippedWeapon->Shoot(StartLocation, Direction);

	UE_LOG(LogTemp, Warning, TEXT("I'm shooting dude"));
}