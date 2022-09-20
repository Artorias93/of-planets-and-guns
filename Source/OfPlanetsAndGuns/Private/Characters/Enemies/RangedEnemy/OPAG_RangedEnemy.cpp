#include "Characters/Enemies/RangedEnemy/OPAG_RangedEnemy.h"
#include <Kismet/KismetMathLibrary.h>
//My Classes
#include "Characters/Enemies/OPAG_AICBase.h"
#include "Weapons/Guns/OPAG_GunBase.h"
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
			AimingLed->SetVisibility(false);
		}
		
		LaserBeam = CreateDefaultSubobject<UNiagaraComponent>("Laser Beam Niagara Component");
		if(LaserBeam)
		{
			LaserBeam->AttachToComponent(Weapon, FAttachmentTransformRules::KeepRelativeTransform);

			//Gets the Niagara System asset for the laser beam
			static ConstructorHelpers::FObjectFinder<UNiagaraSystem> LaserNiagaraSystem(
				TEXT("NiagaraSystem'/Game/OfPlanetsAndGuns/Characters/RangedEnemy/NS_LaserBeam.NS_LaserBeam'"));
			if(LaserNiagaraSystem.Succeeded())
			{
				LaserBeam->SetAsset(LaserNiagaraSystem.Object);
			}
			
			LaserBeam->SetVisibility(false);
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
				FAttachmentTransformRules::SnapToTargetIncludingScale, "RightHandSocketGun");
			EquippedWeapon = Cast<AOPAG_GunBase>(Weapon->GetChildActor());
		}
	}	
}

void AOPAG_RangedEnemy::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (const AOPAG_AICBase* const AIController = Cast<AOPAG_AICBase>(GetController()))
	{
		if(const AOPAG_Player* const Player = Cast<AOPAG_Player>(AIController->GetFocusActor()))
		{
			//TODO: Add a PingPong function to this so it is more realistic?
			//Points the laser beam directly to the player
			LaserBeam->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(LaserBeam->K2_GetComponentLocation(),
				Player->GetActorLocation() + LaserBeamDestinationModifier));

			//TODO: Check out these amazing magic numbers! Hmmm so spicy :P
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
		if(EquippedWeapon->CanShoot() && !GetWorldTimerManager().IsTimerActive(ShotTimerHandle))
		{
			//The first 2 params are multiple "return values" passed by reference.
			GetShootingDirection(StartLocation, Direction, ShootAccuracyTolerance);

			//A random wait from the moment the enemy decides to shoot and when it actually shoots
			const float ShotWaitTime = UKismetMathLibrary::RandomFloatInRange(ShotWaitTimeRandomIntervalBounds[0],
				ShotWaitTimeRandomIntervalBounds[1]);

			if(AimingLed)
				AimingLed->SetVisibility(true);

			GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &AOPAG_RangedEnemy::StartShooting,
				ShotWaitTime);
		}
	}
}

void AOPAG_RangedEnemy::StartShooting()
{
	GetWorldTimerManager().ClearTimer(ShotTimerHandle);
	
	EquippedWeapon->Shoot(StartLocation, Direction, this);

	if(AimingLed)
		AimingLed->SetVisibility(false);
}

void AOPAG_RangedEnemy::Death()
{
	if(AimingLed)
		AimingLed->SetVisibility(false);
	if(LaserBeam)
		LaserBeam->SetVisibility(false);

	if(Weapon)
	{
		Weapon->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

		if(USkeletalMeshComponent* WeaponSkeletalMesh = Cast<USkeletalMeshComponent>(
			Weapon->GetChildActor()->GetComponentByClass(USkeletalMeshComponent::StaticClass())))
		{
			WeaponSkeletalMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
			WeaponSkeletalMesh->SetSimulatePhysics(true);
		}	
	}
	
	Super::Death();
}

void AOPAG_RangedEnemy::GetShootingDirection(FVector& Start, FVector& ShootDirection, float const ShootingAccuracyTolerance /* = 0 */)
{
	if (const AOPAG_AICBase* const AIController = Cast<AOPAG_AICBase>(GetController()))
	{
		if(const AOPAG_Player* const Player = Cast<AOPAG_Player>(AIController->GetFocusActor()))
		{
			if(EquippedWeapon)
			{
				//Gets Start location, in front of the gun
				Start = EquippedWeapon->GetActorLocation() + (GetActorForwardVector() * 50.f);

				//Gets Vector from player to enemy
				const FVector PlayerToSelfVector = Player->GetActorLocation() - Start;

				//Gets not normalized vector with shooting accuracy computations applied
				const float DirectionVectorX =
					PlayerToSelfVector.X + UKismetMathLibrary::RandomFloatInRange(
						-ShootingAccuracyTolerance, ShootingAccuracyTolerance);
				const float DirectionVectorY = PlayerToSelfVector.Y;
				const float DirectionVectorZ =
					PlayerToSelfVector.Z + UKismetMathLibrary::RandomFloatInRange(
						-ShootingAccuracyTolerance, ShootingAccuracyTolerance);
				const FVector DirectionVector = FVector(DirectionVectorX, DirectionVectorY, DirectionVectorZ);
			
				//Gets normalized vector with shooting accuracy computations applied
				ShootDirection = UKismetMathLibrary::Normal(DirectionVector, 0.0001f); //2nd param is tolerance
			}
		}
	}
}

#pragma region Getters & Setters

bool AOPAG_RangedEnemy::GetHasShotSinceSpawning() const
{
	return bHasShotSinceSpawning;
}

void AOPAG_RangedEnemy::SetHasShotSinceSpawning(const bool Value)
{
	this->bHasShotSinceSpawning = Value;
}

UChildActorComponent* AOPAG_RangedEnemy::GetWeapon() const
{
	return Weapon;
}

UStaticMeshComponent* AOPAG_RangedEnemy::GetAimingLed() const
{
	return AimingLed;
}

UNiagaraComponent* AOPAG_RangedEnemy::GetLaserBeam() const
{
	return LaserBeam;
}

#pragma endregion