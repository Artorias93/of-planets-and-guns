// Fill out your copyright notice in the Description page of Project Settings.


#include "Rooms/OPAG_DoorEntranceVolume.h"

#include "Components/BoxComponent.h"
#include "Managers/RoomManager/OPAG_DoorManagerComponent.h"
#include "Managers/RoomManager/OPAG_RoomManager.h"

// Sets default values
AOPAG_DoorEntranceVolume::AOPAG_DoorEntranceVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Volume = CreateDefaultSubobject<UBoxComponent>(TEXT("Volume"));
	RootComponent = Volume;
	Volume->BodyInstance.SetCollisionProfileName(TEXT("Volume"));
	Volume->SetWorldScale3D(FVector::OneVector * 20.f);
	Volume->OnComponentBeginOverlap.AddDynamic(this, &AOPAG_DoorEntranceVolume::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AOPAG_DoorEntranceVolume::BeginPlay()
{
	Super::BeginPlay();
	if (GetWorld()->GetFirstPlayerController()->GetPawn()->IsOverlappingActor(this))
	{
		if (RoomManager)
		{
			RoomManager->EnterRoom();
		}
	}
}

// Called every frame
void AOPAG_DoorEntranceVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AOPAG_DoorEntranceVolume::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == GetWorld()->GetFirstPlayerController()->GetPawn())
	{
		if (RoomManager)
		{
			RoomManager->EnterRoom();
		}
	}
}