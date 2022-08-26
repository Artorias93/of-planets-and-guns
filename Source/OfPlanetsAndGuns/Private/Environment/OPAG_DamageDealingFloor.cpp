// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/OPAG_DamageDealingFloor.h"

#include "Characters/Player/OPAG_Player.h"
#include "Components/BoxComponent.h"

// Sets default values
AOPAG_DamageDealingFloor::AOPAG_DamageDealingFloor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Floor = CreateDefaultSubobject<UBoxComponent>(TEXT("Floor"));
	RootComponent = Floor;
	Floor->CanCharacterStepUpOn = ECB_Yes;
	Floor->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	Floor->SetCanEverAffectNavigation(false);

	FloorVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("Floor Volume"));
	FloorVolume->SetupAttachment(Floor);
	FloorVolume->OnComponentBeginOverlap.AddDynamic(this, &AOPAG_DamageDealingFloor::OnOverlapBegin);
	FloorVolume->OnComponentEndOverlap.AddDynamic(this, &AOPAG_DamageDealingFloor::OnOverlapEnd);
	FloorVolume->SetRelativeLocation(FVector::UpVector * 15.f);
	FloorVolume->SetCanEverAffectNavigation(false);
	TickTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("TickTimeline"));

}

void AOPAG_DamageDealingFloor::TimelineFinished()
{
	if (CharacterToDamage)
	{
		CharacterToDamage->ReceiveDamage(DamagePerTick);
		TickTimeline->PlayFromStart();
	}
}

// Called when the game starts or when spawned
void AOPAG_DamageDealingFloor::BeginPlay()
{
	Super::BeginPlay();

	TickTimeline->SetLooping(false);
	TickTimeline->SetTimelineLength(TickTime);
	TickTimeline->SetTimelineLengthMode(ETimelineLengthMode::TL_TimelineLength);
	OnTimelineFinished.BindUFunction(this, FName("TimelineFinished"));
	TickTimeline->SetTimelineFinishedFunc(OnTimelineFinished);
}

// Called every frame
void AOPAG_DamageDealingFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AOPAG_DamageDealingFloor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const auto PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return;
	AOPAG_Player* Player = PC->GetPawn<AOPAG_Player>();
	if (OtherActor == Player)
	{
		CharacterToDamage = Player;
		TickTimeline->Play();
	}
}

void AOPAG_DamageDealingFloor::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	const auto PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return;
	const AOPAG_Player* Player = PC->GetPawn<AOPAG_Player>();
	if (OtherActor == Player)
	{
		CharacterToDamage = nullptr;
		TickTimeline->Stop();
	}
}
