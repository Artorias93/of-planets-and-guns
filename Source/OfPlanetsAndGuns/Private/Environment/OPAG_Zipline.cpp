// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/OPAG_Zipline.h"

// Sets default values
AOPAG_Zipline::AOPAG_Zipline()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bCanPlayerInteract = false;
	StartingPoint = nullptr;
	EndingPoint = nullptr;
	PlayerRef = nullptr;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root "));
	Cable = CreateDefaultSubobject<UCableComponent>(TEXT("Zipline Cable"));
	

	Root->SetupAttachment(GetRootComponent());
	Cable->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void AOPAG_Zipline::BeginPlay()
{
	Super::BeginPlay();
	SetUpZipline();

}


// Called every frame
void AOPAG_Zipline::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AOPAG_Zipline::Interact_Implementation()
{
	if (!bCanPlayerInteract) return;
	UE_LOG(LogTemp, Warning, TEXT("INTERACT"));

	float deltaDistance = FVector::Distance(StartingPoint->PlayerAttachPoint->GetComponentLocation(), EndingPoint->PlayerAttachPoint->GetComponentLocation());
	float deltaTime = deltaDistance / 1000;
	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;

	UKismetSystemLibrary::MoveComponentTo(PlayerRef->GetRootComponent(), StartingPoint->PlayerAttachPoint->GetComponentLocation(), PlayerRef->GetRootComponent()->GetRelativeRotation(), false, false, 0.3, false, EMoveComponentAction::Type::Move, LatentInfo);
	UKismetSystemLibrary::MoveComponentTo(PlayerRef->GetRootComponent(), EndingPoint->PlayerAttachPoint->GetComponentLocation(), PlayerRef->GetRootComponent()->GetRelativeRotation(), false, true, deltaTime , false, EMoveComponentAction::Type::Move , LatentInfo);
}

bool AOPAG_Zipline::CanInteract_Implementation()
{
	return bCanPlayerInteract;
}

void AOPAG_Zipline::SetUpZipline() {
	if (PointA && PointB) {
		Cable->AttachToComponent(PointA->CableAttachPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		Cable->SetAttachEndToComponent(PointB->CableAttachPoint);
		PointA->InteractableArea->BodyInstance.SetCollisionProfileName(TEXT("Volume"));
		PointB->InteractableArea->BodyInstance.SetCollisionProfileName(TEXT("Volume"));
		PointA->InteractableArea->OnComponentBeginOverlap.AddDynamic(this, &AOPAG_Zipline::OverlapPointABegin);
		PointB->InteractableArea->OnComponentBeginOverlap.AddDynamic(this, &AOPAG_Zipline::OverlapPointBBegin);
		PointA->InteractableArea->OnComponentEndOverlap.AddDynamic(this, &AOPAG_Zipline::OverlapEnd);
		PointB->InteractableArea->OnComponentEndOverlap.AddDynamic(this, &AOPAG_Zipline::OverlapEnd);
	}
}


void AOPAG_Zipline::OverlapReset() {
	StartingPoint = nullptr;
	EndingPoint = nullptr;
	PlayerRef = nullptr;
	bCanPlayerInteract = false;
}

void AOPAG_Zipline::OverlapPointABegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor == GetWorld()->GetFirstPlayerController()->GetPawn()){
		if (StartingPoint == nullptr && PlayerRef == nullptr) {
			StartingPoint = PointA;
			EndingPoint = PointB;
			PlayerRef = (AOPAG_Player*)OtherActor;
			bCanPlayerInteract = true;
		}
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Collision A Begin"));
}

void AOPAG_Zipline::OverlapPointBBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor == GetWorld()->GetFirstPlayerController()->GetPawn()) {
		if (StartingPoint == nullptr && PlayerRef == nullptr) {
			StartingPoint = PointB;
			EndingPoint = PointA;
			PlayerRef = (AOPAG_Player*)OtherActor;
			bCanPlayerInteract = true;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Collision B Begin"));
}


void AOPAG_Zipline::OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	OverlapReset();
}