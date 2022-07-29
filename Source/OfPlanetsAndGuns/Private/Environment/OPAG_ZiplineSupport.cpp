// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/OPAG_ZiplineSupport.h"

// Sets default values
AOPAG_ZiplineSupport::AOPAG_ZiplineSupport()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SupportMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	InteractableArea = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	CableAttachPoint = CreateDefaultSubobject<USceneComponent>(TEXT("CableAttachPoint"));
	PlayerAttachPoint = CreateDefaultSubobject<USceneComponent>(TEXT("PlayerAttachPoint"));

	Root->SetupAttachment(GetRootComponent());
	SupportMesh->SetupAttachment(Root);
	InteractableArea->SetupAttachment(Root);
	CableAttachPoint->SetupAttachment(Root);
	PlayerAttachPoint->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void AOPAG_ZiplineSupport::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOPAG_ZiplineSupport::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

