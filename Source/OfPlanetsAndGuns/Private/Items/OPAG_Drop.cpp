// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/OPAG_Drop.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

AOPAG_Drop::AOPAG_Drop()
{
	PrimaryActorTick.bCanEverTick = true;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	if (Sphere)
	{
		RootComponent = Sphere;
	}

	DropMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DropMesh"));
	if (DropMesh) 
	{
		DropMesh->SetupAttachment(RootComponent);
		DropMesh->SetCollisionProfileName("Drop");
	}
}

void AOPAG_Drop::BeginPlay()
{
	Super::BeginPlay();
	
}

void AOPAG_Drop::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AOPAG_Drop::ApplyForce_Implementation()
{
	Sphere->SetSimulatePhysics(true);
	const FVector Force = Sphere->GetUpVector() * 20000.f + Sphere->GetForwardVector() * 20000.f;
	Sphere->AddForce(Force, NAME_None, true);
}

