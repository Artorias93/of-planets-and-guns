// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Modules/OPAG_AccessoryBase.h"

// Sets default values
AOPAG_AccessoryBase::AOPAG_AccessoryBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AOPAG_AccessoryBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOPAG_AccessoryBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

