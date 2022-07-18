// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/OPAG_SimpleDoor.h"

#include "Components/BoxComponent.h"
#include "Components/TimelineComponent.h"

AOPAG_SimpleDoor::AOPAG_SimpleDoor()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	Frame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Frame"));
	Frame->SetupAttachment(RootComponent);
	Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
	Door->SetupAttachment(RootComponent);
	CollisionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionVolume"));
	CollisionVolume->SetupAttachment(RootComponent);
	CollisionVolume->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	Timeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Movement Timeline"));
}

void AOPAG_SimpleDoor::TimelineCallback(float Height) const
{
	Door->SetRelativeLocation(InitialDoorLocation + FVector::UpVector * Height);
}

void AOPAG_SimpleDoor::BeginPlay()
{
	Super::BeginPlay();
	InitialDoorLocation = Door->GetRelativeLocation();
	bIsOpen = false;

	if (MovementCurve != nullptr)
	{
		FOnTimelineFloat onTimelineCallback;
		onTimelineCallback.BindUFunction(this, FName{ TEXT("TimelineCallback") });
		Timeline->AddInterpFloat(MovementCurve, onTimelineCallback);
		Timeline->RegisterComponent();
	}

	OpenDoor();
}

void AOPAG_SimpleDoor::OpenDoor_Implementation()
{
	CollisionVolume->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (Timeline != nullptr && !bIsOpen)
	{
		Timeline->PlayFromStart();
		bIsOpen = true;
	}

}

void AOPAG_SimpleDoor::CloseDoor_Implementation()
{
	CollisionVolume->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	if (Timeline != nullptr && bIsOpen)
	{
		Timeline->ReverseFromEnd();
		bIsOpen = false;
	}
}
