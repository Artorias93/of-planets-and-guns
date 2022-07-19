// Fill out your copyright notice in the Description page of Project Settings.


#include "Rooms/OPAG_RoomAttachPoint.h"

#include "Components/BillboardComponent.h"
#include "Components/ArrowComponent.h"
#include "Managers/RoomManager/OPAG_RoomManager.h"

// Sets default values
AOPAG_RoomAttachPoint::AOPAG_RoomAttachPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Gizmo"));
	RootComponent = Billboard;
	Billboard->bIsScreenSizeScaled = true;
	Billboard->ScreenSize = -0.005f;

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Forward Arrow"));
	Arrow->SetupAttachment(RootComponent);
	Arrow->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));
	Arrow->ArrowColor = FColor::Yellow;
	Arrow->ArrowSize = 50.f;
	Arrow->ArrowLength = 100.f;
	Arrow->ScreenSize = 0.0005f;
	Arrow->bIsScreenSizeScaled = true;

    const auto StartSpriteFinder = ConstructorHelpers::FObjectFinder<UTexture2D>(
		TEXT("Texture2D'/Engine/MobileResources/HUD/AnalogHat.AnalogHat'"));
	if (StartSpriteFinder.Succeeded()) StartSprite = StartSpriteFinder.Object;

    const auto ExitSpriteFinder = ConstructorHelpers::FObjectFinder<UTexture2D>(
		TEXT("Texture2D'/Engine/EditorMaterials/Anchor.Anchor'"));
	if (ExitSpriteFinder.Succeeded()) ExitSprite = ExitSpriteFinder.Object;

}

// Called when the game starts or when spawned
void AOPAG_RoomAttachPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOPAG_RoomAttachPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AOPAG_RoomAttachPoint::UpdateAttachPointData(AOPAG_RoomManager* const RoomManager)
{
	Billboard->SetSprite(bIsStart ? StartSprite : ExitSprite);

	RoomOffset = GetActorTransform();

	if (bIsStart)
		RoomManager->Entrance = this;
	else
		RoomManager->Exits.AddUnique(this);
}

