#include "Items/OPAG_Drop.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Math/Color.h"

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

	HighlightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HighlightMesh"));
	if (HighlightMesh)
	{
		HighlightMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		HighlightMesh->SetSimulatePhysics(false);
		//HighlightMesh->SetCollisionProfileName("Drop");

		HighlightMesh->SetupAttachment(DropMesh);

		HighlightMesh->SetWorldScale3D(FVector(0, 0, 0));
	}

	CurrentHighlightIncreaseSizeFactor = 0;
}

void AOPAG_Drop::BeginPlay()
{
	Super::BeginPlay();
	
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AOPAG_Drop::FloatingDrop, DisablePhysicAfterSeconds, false);
}

void AOPAG_Drop::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentHighlightIncreaseSizeFactor < HighlightIncreaseSizeFactor)
	{
		CurrentHighlightIncreaseSizeFactor += DeltaTime;
		HighlightMesh->SetWorldScale3D(FVector(CurrentHighlightIncreaseSizeFactor, CurrentHighlightIncreaseSizeFactor, CurrentHighlightIncreaseSizeFactor));
	}
}

void AOPAG_Drop::ApplyForce_Implementation(const FVector Force)
{
	Sphere->SetSimulatePhysics(true);
	Sphere->AddForce(Force, NAME_None, true);
}

void AOPAG_Drop::FloatingDrop()
{
	DropMesh->SetSimulatePhysics(false);
	Sphere->SetSimulatePhysics(false);
}