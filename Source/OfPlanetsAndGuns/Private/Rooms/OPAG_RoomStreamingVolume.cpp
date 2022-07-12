// Fill out your copyright notice in the Description page of Project Settings.


#include "Rooms/OPAG_RoomStreamingVolume.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/OPAG_EventManagerSubsystem.h"
#include "Managers/OPAG_GameInstance.h"

// Sets default values
AOPAG_RoomStreamingVolume::AOPAG_RoomStreamingVolume()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Volume = CreateDefaultSubobject<UBoxComponent>("Volume");
	RootComponent = Volume;
	Volume->OnComponentBeginOverlap.AddDynamic(this, &AOPAG_RoomStreamingVolume::OnOverlapBegin);
	Volume->OnComponentEndOverlap.AddDynamic(this, &AOPAG_RoomStreamingVolume::OnOverlapEnd);
}

// Called when the game starts or when spawned
void AOPAG_RoomStreamingVolume::BeginPlay()
{
	Super::BeginPlay();
	UOPAG_GameInstance* GI = Cast<UOPAG_GameInstance>(GetGameInstance());
	if (GI)
	{
		EventManager = GI->GetEventManager();
	}
}

// Called every frame
void AOPAG_RoomStreamingVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AOPAG_RoomStreamingVolume::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GetWorld() && GetWorld()->GetFirstPlayerController() &&
		OtherActor == GetWorld()->GetFirstPlayerController()->GetPawnOrSpectator())
		EventManager->EnteredRoomStreamingVolume.Broadcast(this);
}

void AOPAG_RoomStreamingVolume::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (GetWorld() && GetWorld()->GetFirstPlayerController() &&
		OtherActor == GetWorld()->GetFirstPlayerController()->GetPawnOrSpectator())
		EventManager->LeftRoomStreamingVolume.Broadcast();
}
