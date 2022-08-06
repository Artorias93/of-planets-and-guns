#include "Characters/Enemies/NormalTurret/OPAG_NormalTurret.h"
#include "Characters/Enemies/OPAG_AICBase.h"
#include "Characters/Player/OPAG_Player.h"
#include <Engine.h>
#include <Kismet/KismetMathLibrary.h>

AOPAG_NormalTurret::AOPAG_NormalTurret()
{
	PrimaryActorTick.bCanEverTick = true;

	//ExplosionImpulseRandomRange = 900.f;

	Muzzle = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle"));
	if (Muzzle)
	{
		Muzzle->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "TurretMuzzleSocket");
	}
}

void AOPAG_NormalTurret::Attack()
{
	UGameplayStatics::PlaySound2D(GetWorld(), ShootSound);
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

//Returns a random vector in a range (the bounds of the range can be set in editor)
FVector AOPAG_NormalTurret::GetRandomImpulseVector() const
{
	return FVector(FMath::FRandRange(-900, 900),
		FMath::FRandRange(-900, 900),
		FMath::FRandRange(-900, 900));
}

FRotator AOPAG_NormalTurret::GetTurretHeadRotation() const
{
	return TurretHeadRotation;
}

void AOPAG_NormalTurret::Tick(float DeltaSeconds)
{
	if (const AOPAG_AICBase* const AIController = Cast<AOPAG_AICBase>(GetController()))
	{
		if (const AOPAG_Player* const Player = Cast<AOPAG_Player>(AIController->GetFocusActor()))
		{
			// Since the Anim BP sets the rotation in "ADD" mode, we need to subtract the whole actor rotation
			TurretHeadRotation = UKismetMathLibrary::FindLookAtRotation(Muzzle->GetComponentLocation(),
				Player->GetActorLocation()) - GetActorRotation();;	
		}
	}
}