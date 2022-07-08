// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Guns/OPAG_GunBase.h"

// Sets default values
AOPAG_GunBase::AOPAG_GunBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AOPAG_GunBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOPAG_GunBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

