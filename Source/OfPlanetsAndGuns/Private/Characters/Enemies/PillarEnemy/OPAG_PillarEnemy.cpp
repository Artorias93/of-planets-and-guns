// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/PillarEnemy/OPAG_PillarEnemy.h"
#include "Components/StaticMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Managers/OPAG_EventManagerSubsystem.h"
#include "Kismet/GameplayStatics.h"

AOPAG_PillarEnemy::AOPAG_PillarEnemy()
{
	PillarMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	PillarMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

void AOPAG_PillarEnemy::Death()
{
	if (EMS != nullptr)
	{
		EMS->DestroyedPillar.Broadcast();
	}

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
