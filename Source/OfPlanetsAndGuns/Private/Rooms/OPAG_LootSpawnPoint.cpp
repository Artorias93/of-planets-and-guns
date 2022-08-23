// Fill out your copyright notice in the Description page of Project Settings.


#include "Rooms/OPAG_LootSpawnPoint.h"

#include "Components/BillboardComponent.h"

// Sets default values
AOPAG_LootSpawnPoint::AOPAG_LootSpawnPoint()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Gizmo"));
	RootComponent = Billboard;
	Billboard->bIsScreenSizeScaled = true;
	Billboard->ScreenSize = -0.002f;

	const auto SpriteFinder = ConstructorHelpers::FObjectFinder<UTexture2D>(
		TEXT("Texture2D'/Game/OfPlanetsAndGuns/UI/Billboards/LootSpawnerBillboard.LootSpawnerBillboard'"));
	if (SpriteFinder.Succeeded()) Billboard->SetSprite(SpriteFinder.Object);
}

// Called when the game starts or when spawned
void AOPAG_LootSpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOPAG_LootSpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

