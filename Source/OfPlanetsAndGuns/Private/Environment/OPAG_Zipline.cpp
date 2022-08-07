// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/OPAG_Zipline.h"
#include "Managers/OPAG_GameInstance.h"
#include "Managers/OPAG_EventManagerSubsystem.h"


// Sets default values
AOPAG_Zipline::AOPAG_Zipline()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlayerRef = nullptr;
	bCanPlayerInteract = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root "));
	Cable = CreateDefaultSubobject<UCableComponent>(TEXT("Zipline Cable"));
	PointA = CreateDefaultSubobject<USceneComponent>(TEXT("PointA"));
	PointB = CreateDefaultSubobject<USceneComponent>(TEXT("PointB"));
	SupportMeshA = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SupportMesh A"));
	SupportMeshB = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SupportMesh B"));
	InteractableAreaA = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractableArea A"));
	InteractableAreaB = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractableArea B"));
	CableAttachPointA = CreateDefaultSubobject<USceneComponent>(TEXT("CableAttachPoint A"));
	PlayerAttachPointA = CreateDefaultSubobject<USceneComponent>(TEXT("PlayerAttachPoint A"));
	CableAttachPointB = CreateDefaultSubobject<USceneComponent>(TEXT("CableAttachPoint B"));
	PlayerAttachPointB = CreateDefaultSubobject<USceneComponent>(TEXT("PlayerAttachPoint B"));



	Root->SetupAttachment(GetRootComponent());
	Cable->SetupAttachment(Root);
	PointA->SetupAttachment(Root);
	PointB->SetupAttachment(Root);
	SupportMeshA->SetupAttachment(PointA);
	InteractableAreaA->SetupAttachment(PointA);
	PlayerAttachPointA->SetupAttachment(PointA);
	CableAttachPointA->SetupAttachment(PointA);
	SupportMeshB->SetupAttachment(PointB);
	InteractableAreaB->SetupAttachment(PointB);
	PlayerAttachPointB->SetupAttachment(PointB);
	CableAttachPointB->SetupAttachment(PointB);

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

	//if (StartingPoint == nullptr || EndingPoint == nullptr || PlayerRef == nullptr) return;
	UE_LOG(LogTemp, Warning, TEXT("INTERACT"));

	float deltaDistance = FVector::Distance(StartingPoint, EndingPoint);
	float deltaTime = deltaDistance / 1000;
	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;

	UKismetSystemLibrary::MoveComponentTo(PlayerRef->GetRootComponent(), StartingPoint, PlayerRef->GetRootComponent()->GetRelativeRotation(), false, false, 0.3, false, EMoveComponentAction::Type::Move, LatentInfo);
	UKismetSystemLibrary::MoveComponentTo(PlayerRef->GetRootComponent(), EndingPoint, PlayerRef->GetRootComponent()->GetRelativeRotation(), false, true, deltaTime , false, EMoveComponentAction::Type::Move , LatentInfo);
	
}

bool AOPAG_Zipline::CanInteract_Implementation() {
	return bCanPlayerInteract;
}


void AOPAG_Zipline::SetUpZipline() {
	Cable->AttachToComponent(CableAttachPointA, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	Cable->SetAttachEndToComponent(CableAttachPointB);
	InteractableAreaA->BodyInstance.SetCollisionProfileName(TEXT("Volume"));
	InteractableAreaB->BodyInstance.SetCollisionProfileName(TEXT("Volume"));
	InteractableAreaA->OnComponentBeginOverlap.AddDynamic(this, &AOPAG_Zipline::OverlapPointABegin);
	InteractableAreaB->OnComponentBeginOverlap.AddDynamic(this, &AOPAG_Zipline::OverlapPointBBegin);
	InteractableAreaA->OnComponentEndOverlap.AddDynamic(this, &AOPAG_Zipline::OverlapReset);
	InteractableAreaB->OnComponentEndOverlap.AddDynamic(this, &AOPAG_Zipline::OverlapReset);
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AOPAG_Player::StaticClass(), FoundActors);
	PlayerRef = (AOPAG_Player*)FoundActors[0];
}

void AOPAG_Zipline::OverlapPointABegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor == GetWorld()->GetFirstPlayerController()->GetPawn()){
		StartingPoint = PlayerAttachPointA->GetComponentLocation();
		EndingPoint = PlayerAttachPointB->GetComponentLocation();
		//PlayerRef = (AOPAG_Player*)OtherActor;
		bCanPlayerInteract = true;
		UE_LOG(LogTemp, Warning, TEXT("Collision A Begin Step 2"));

	}
	UE_LOG(LogTemp, Warning, TEXT("Collision A Begin"));
}

void AOPAG_Zipline::OverlapPointBBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor == GetWorld()->GetFirstPlayerController()->GetPawn()) {
		StartingPoint = PlayerAttachPointB->GetComponentLocation();
		EndingPoint = PlayerAttachPointA->GetComponentLocation();
		//PlayerRef = (AOPAG_Player*)OtherActor;
		bCanPlayerInteract = true;
	}
	UE_LOG(LogTemp, Warning, TEXT("Collision B Begin"));
}


void AOPAG_Zipline::OverlapReset(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	bCanPlayerInteract = false;
}
