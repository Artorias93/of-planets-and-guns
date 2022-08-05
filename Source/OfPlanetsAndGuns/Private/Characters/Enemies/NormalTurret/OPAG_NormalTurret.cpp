// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/NormalTurret/OPAG_NormalTurret.h"
#include "Characters/Enemies/OPAG_AICBase.h"
#include "Characters/Player/OPAG_Player.h"
#include <Engine.h>
#include <Kismet/KismetMathLibrary.h>

AOPAG_NormalTurret::AOPAG_NormalTurret()
{
	PrimaryActorTick.bCanEverTick = true;

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

	Super::Death();
}

FRotator AOPAG_NormalTurret::GetTurretHeadRotation() const
{
	return TurretHeadRotation;
}

void AOPAG_NormalTurret::Tick(float DeltaSeconds)
{
	AOPAG_AICBase* AIController = Cast<AOPAG_AICBase>(GetController());

	if (AIController)
	{
		AOPAG_Player* Player = Cast<AOPAG_Player>(AIController->GetFocusActor());

		if (Player)
		{
			// Since the Anim BP set the rotation in mode "ADD", then we need to subtract the whole actor rotation
			FRotator NewRotator = UKismetMathLibrary::FindLookAtRotation(Muzzle->GetComponentLocation(), Player->GetActorLocation()) - GetActorRotation();
			TurretHeadRotation = NewRotator;			
		}
	}
}

