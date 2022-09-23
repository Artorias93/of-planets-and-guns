#include "Characters/Enemies/NormalTurret/OPAG_NormalTurret.h"
#include "Characters/Enemies/OPAG_AICBase.h"
#include "Characters/Player/OPAG_Player.h"
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/GameplayStatics.h>
#include "DrawDebugHelpers.h"

AOPAG_NormalTurret::AOPAG_NormalTurret()
{
	PrimaryActorTick.bCanEverTick = true;

	Muzzle = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle"));
	if (Muzzle)
	{
		Muzzle->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "TurretMuzzleSocket");
	}

	LaserBeam = CreateDefaultSubobject<UNiagaraComponent>("Laser Beam Niagara Component");
	if (LaserBeam)
	{
		LaserBeam->AttachToComponent(Muzzle, FAttachmentTransformRules::KeepRelativeTransform);

		//Gets the Niagara System asset for the laser beam
		static ConstructorHelpers::FObjectFinder<UNiagaraSystem> LaserNiagaraSystem(
			TEXT("NiagaraSystem'/Game/OfPlanetsAndGuns/Characters/RangedEnemy/NS_LaserBeam.NS_LaserBeam'"));
		if (LaserNiagaraSystem.Succeeded())
		{
			LaserBeam->SetAsset(LaserNiagaraSystem.Object);
		}

		LaserBeam->SetVisibility(false);
	}
}

void AOPAG_NormalTurret::Attack()
{	
	if (!GetWorldTimerManager().IsTimerActive(ShotTimerHandle))
	{
		GetShootingDirection(StartLocation, TurretShootDirection, 0.f);

		GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &AOPAG_NormalTurret::StartShooting,
			ShootingTimer);
	}
}

void AOPAG_NormalTurret::Death()
{
	UGameplayStatics::SpawnEmitterAttached(
		DeathParticle,
		this->GetDefaultAttachComponent(),
		NAME_None,
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		EAttachLocation::KeepRelativeOffset,
		true,
		EPSCPoolMethod::None,
		true
	);

	//Breaks constraints on all the bones of the skeletal mesh and gives them an impulse in a random direction,
	//simulating an explosion
	GetMesh()->BreakConstraint(GetRandomImpulseVector(), FVector::ZeroVector, "SK_Prop_Turret_Base_Single_01");
	GetMesh()->BreakConstraint(GetRandomImpulseVector(), FVector::ZeroVector, "SK_Prop_Turret_Gattling_Base_01");
	GetMesh()->BreakConstraint(GetRandomImpulseVector(), FVector::ZeroVector, "SK_Prop_Turret_Gattling_Barrel_01");

	Super::Death();
}

void AOPAG_NormalTurret::StartShooting()
{
	if (ShotTimerHandle.IsValid() && GetWorld())
		GetWorld()->GetTimerManager().ClearTimer(ShotTimerHandle);

	ShootLineTrace(StartLocation, TurretShootDirection);
	
	UGameplayStatics::PlaySound2D(GetWorld(), ShootSound);

	// activate shoot effect
	UGameplayStatics::SpawnEmitterAttached(
		MuzzleParticle,
		Muzzle,
		NAME_None,
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		EAttachLocation::KeepRelativeOffset,
		true,
		EPSCPoolMethod::None,
		true
	);
}

void AOPAG_NormalTurret::GetShootingDirection(FVector& Start, FVector& ShootDirection, float const ShootingAccuracyTolerance /* = 0 */)
{
	if (const AOPAG_AICBase* const AIController = Cast<AOPAG_AICBase>(GetController()))
	{
		if (const AOPAG_Player* const Player = Cast<AOPAG_Player>(AIController->GetFocusActor()))
		{
			//Gets Start location, in front of the gun
			Start = Muzzle->GetComponentLocation();

			//Gets Vector from player to enemy
			const FVector PlayerToSelfVector = Player->GetActorLocation() - Start;

			float EnemyToPlayerLenght = PlayerToSelfVector.Size();

			// get the two tangent vectors to the one from the player to the enemy (as remaining axis)
			FVector TangentVector1 = FVector::CrossProduct(PlayerToSelfVector, FVector::UpVector);
			TangentVector1.Normalize();
			FVector TangentVector2 = FVector::CrossProduct(PlayerToSelfVector, TangentVector1);
			TangentVector2.Normalize();

			float RandomAngle1 = UKismetMathLibrary::RandomFloatInRange(
				-RandomAngleShotBounds,
				RandomAngleShotBounds
			);

			float RandomAngle2 = UKismetMathLibrary::RandomFloatInRange(
				-RandomAngleShotBounds,
				RandomAngleShotBounds
			);

			float RandomFactor1 = FMath::Tan(((RandomAngle1 * PI) / 180)) * EnemyToPlayerLenght;
			float RandomFactor2 = FMath::Tan(((RandomAngle2 * PI) / 180)) * EnemyToPlayerLenght;

			TangentVector1 *= RandomFactor1;
			TangentVector2 *= RandomFactor2;

			FVector ShotDirectionResult = PlayerToSelfVector + TangentVector1 + TangentVector2;

			//Gets normalized vector with shooting accuracy computations applied
			ShootDirection = ShotDirectionResult; //2nd param is tolerance
		}
	}
}

void AOPAG_NormalTurret::ShootLineTrace(FVector StartShootLocation, FVector Direction)
{
	// Initialize out of hit and if check is true (hit object in scene) so the actor is cast to character and control the return value
	FHitResult OutHit;
	TArray<AActor*> ActorsToIgnore = GetAllActorsToIgnore();

	const bool bIsHit = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), StartShootLocation, StartShootLocation + Direction * 2000, ObjectTypesQuery, false,
		ActorsToIgnore, EDrawDebugTrace::None, OutHit, true, FLinearColor::Red, FLinearColor::Green, 5.f);

	// If Actor is inheritance by character base, invoke take damage
	if (bIsHit)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, OutHit.Location, FRotator::ZeroRotator, true, EPSCPoolMethod::None, true);

		AOPAG_CharacterBase* Character = Cast<AOPAG_CharacterBase>(OutHit.GetActor());
		if (Character)
		{
			Character->ReceiveDamage(Damage, this);
		}
	}
}

TArray<AActor*> AOPAG_NormalTurret::GetAllActorsToIgnore()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AOPAG_Enemy::StaticClass(), FoundActors);

	return FoundActors;
}

//Returns a random vector in a range (the bounds of the range can be set in editor)
FVector AOPAG_NormalTurret::GetRandomImpulseVector() const
{
	return FVector(FMath::FRandRange(-ExplosionPower, ExplosionPower),
		FMath::FRandRange(-ExplosionPower, ExplosionPower),
		FMath::FRandRange(-ExplosionPower, ExplosionPower));
}

UNiagaraComponent* AOPAG_NormalTurret::GetLaserBeam() const
{
	return LaserBeam;
}
