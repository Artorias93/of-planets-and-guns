// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/PillarEnemy/OPAG_PillarEnemy.h"
#include "Components/StaticMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Managers/OPAG_EventManagerSubsystem.h"

AOPAG_PillarEnemy::AOPAG_PillarEnemy()
{
	PillarMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	PillarMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

void AOPAG_PillarEnemy::Death()
{

	if (EMS != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("Destroyed Pillar"));
		EMS->DestroyedPillar.Broadcast();
	}

	Super::Death();
}
