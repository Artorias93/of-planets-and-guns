// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/MageEnemy/OPAG_MageEnemy.h"
#include "Components/ChildActorComponent.h"

AOPAG_MageEnemy::AOPAG_MageEnemy()
{
	ForceField = CreateDefaultSubobject<UChildActorComponent>("ForceField");
	ForceField->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	
}

void AOPAG_MageEnemy::BeginPlay()
{

}

void AOPAG_MageEnemy::Attack()
{

}

void AOPAG_MageEnemy::Death()
{

}
