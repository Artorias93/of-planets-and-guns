// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/OPAG_LootBox.h"

AOPAG_LootBox::AOPAG_LootBox()
{
	PrimaryActorTick.bCanEverTick = true;

	Crate = CreateDefaultSubobject<UStaticMeshComponent>("Crate");
	if (Crate)
		RootComponent = Crate;

	Lid = CreateDefaultSubobject<UStaticMeshComponent>("Lid");
	if (Lid)
		Lid->SetupAttachment(RootComponent);

	LootSpawnPoint = CreateDefaultSubobject<UStaticMeshComponent>("LootSpawnPoint");
	if (LootSpawnPoint)
		LootSpawnPoint->SetupAttachment(RootComponent);
}

void AOPAG_LootBox::BeginPlay()
{
	Super::BeginPlay();
	
}

void AOPAG_LootBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AOPAG_LootBox::Interact_Implementation()
{

}

bool AOPAG_LootBox::CanInteract_Implementation()
{
	return bIsInteractable;
}

