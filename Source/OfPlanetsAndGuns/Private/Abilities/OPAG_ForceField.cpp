// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/OPAG_ForceField.h"
#include "Components/StaticMeshComponent.h"

AOPAG_ForceField::AOPAG_ForceField()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("'/Game/OfPlanetsAndGuns/Characters/MageEnemy/S_ForceField.S_ForceField'"));
	if (Mesh.Succeeded())
	{
		AbilityMesh->SetStaticMesh(Mesh.Object);
		AbilityMesh->SetRelativeScale3D(FVector(3.5, 3.5, 3.5));
	}
}