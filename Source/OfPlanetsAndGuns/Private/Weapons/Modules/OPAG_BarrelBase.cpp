#include "Weapons/Modules/OPAG_BarrelBase.h"
#include "Components/ChildActorComponent.h"

AOPAG_BarrelBase::AOPAG_BarrelBase()
{
	MuzzleComponent = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleComponent"));
	MuzzleComponent->SetRelativeRotation(FRotator(0, 90.f, 0));
	MuzzleComponent->SetupAttachment(RootComponent);

	// Initialize Parameters
	Rarity = EModuleRarity::Common;
	Type = EModuleTypes::Barrel;
}

void AOPAG_BarrelBase::BeginPlay()
{
	Super::BeginPlay();
}

void AOPAG_BarrelBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}