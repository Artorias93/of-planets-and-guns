// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/PillarEnemy/OPAG_PillarEnemy.h"
#include "Components/StaticMeshComponent.h"

AOPAG_PillarEnemy::AOPAG_PillarEnemy()
{
	PillarMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	PillarMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

void AOPAG_PillarEnemy::Death()
{
	if (PillarMesh)
	{
		PillarMesh->K2_DetachFromComponent(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld);
		PillarMesh->SetSimulatePhysics(true);
		PillarMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	}
	
	Super::Death();
}
