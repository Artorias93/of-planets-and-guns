// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/OPAG_ForceField.h"
#include "Components/StaticMeshComponent.h"

AOPAG_ForceField::AOPAG_ForceField()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}