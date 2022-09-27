#include "Items/OPAG_Drop.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Math/Color.h"
#include "Curves/CurveVector.h"

AOPAG_Drop::AOPAG_Drop()
{
	PrimaryActorTick.bCanEverTick = true;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	if (Sphere)
	{
		RootComponent = Sphere;
		Sphere->SetSimulatePhysics(false);
	}

	DropMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DropMesh"));
	if (DropMesh) 
	{
		DropMesh->SetupAttachment(RootComponent);
		DropMesh->SetCollisionProfileName("Drop");
		DropMesh->SetSimulatePhysics(false);
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

	DropTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("DropTimeline"));

	static ConstructorHelpers::FObjectFinder<UCurveVector> CurveVector(TEXT("'/Game/OfPlanetsAndGuns/Interactables/Curve_Drop_Vector.Curve_Drop_Vector'"));
	if (CurveVector.Succeeded())
	{
		CurveDropVector = CurveVector.Object;
	}

	CurrentHighlightIncreaseSizeFactor = 0;
}

void AOPAG_Drop::BeginPlay()
{
	Super::BeginPlay();

	DropTimeline->SetLooping(false);
	DropTimeline->SetTimelineLengthMode(ETimelineLengthMode::TL_TimelineLength);

	OnDropTimelineProgress.BindUFunction(this, FName("DropTimelineProgress"));
	DropTimeline->AddInterpVector(CurveDropVector, OnDropTimelineProgress, FName("DropTimelinePropertyName"));	
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

void AOPAG_Drop::DropTimelineProgress(FVector Value)
{
	FVector NewLocation = SpawnLocation;
	NewLocation.X += RandomDropDir.X * Value.X * DropCurveXFactor;
	NewLocation.Y += RandomDropDir.Y * Value.X * DropCurveXFactor;
	NewLocation.Z += Value.Z * DropCurveZFactor;

	SetActorLocation(NewLocation);
}

void AOPAG_Drop::StartDropTimeline()
{
	SpawnLocation = GetActorLocation();

	// Find a random angle in order to determine the direction where the module will drop
	float RandomAngle = FMath::DegreesToRadians(FMath::RandRange(0, 360));
	RandomDropDir.X = FMath::Sin(RandomAngle);
	RandomDropDir.Y = FMath::Cos(RandomAngle);

	DropTimeline->PlayFromStart();
}
