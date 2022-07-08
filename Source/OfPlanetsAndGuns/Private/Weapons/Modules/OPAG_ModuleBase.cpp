// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Modules/OPAG_ModuleBase.h"

// Sets default values
AOPAG_ModuleBase::AOPAG_ModuleBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AOPAG_ModuleBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOPAG_ModuleBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

