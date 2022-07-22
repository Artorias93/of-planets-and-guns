// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/RoomManager/OPAG_DoorManagerComponent.h"

#include "Environment/OPAG_DoorBase.h"
#include "Managers/RoomManager/OPAG_RoomManager.h"

// Sets default values for this component's properties
UOPAG_DoorManagerComponent::UOPAG_DoorManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOPAG_DoorManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UOPAG_DoorManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, 
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UOPAG_DoorManagerComponent::CloseDoors()
{
	for (AOPAG_DoorBase* Door : Doors)
	{
		if (Door)
		{
			Door->CloseDoor();
		}
	}
}

void UOPAG_DoorManagerComponent::OpenDoors()
{
	for (AOPAG_DoorBase* Door : Doors)
	{
		if (Door)
		{
			Door->OpenDoor();
		}
	}
}

