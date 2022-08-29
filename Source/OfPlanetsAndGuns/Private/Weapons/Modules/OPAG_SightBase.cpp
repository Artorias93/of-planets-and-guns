#include "Weapons/Modules/OPAG_SightBase.h"
#include "Camera/CameraComponent.h"

AOPAG_SightBase::AOPAG_SightBase()
{
	SightCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SightCamera"));
	SightCameraComponent->SetRelativeLocation(FVector(-0.000095f, -18.851955f, 6.571584f));
	SightCameraComponent->SetRelativeRotation(FRotator(0, 90.f, 0));
	SightCameraComponent->SetRelativeScale3D(FVector(.2f, .2f, .2f));
	SightCameraComponent->SetFieldOfView(50.f);
	SightCameraComponent->SetupAttachment(RootComponent);

	// Initialize Parameters
	Rarity = EModuleRarity::Common;
	Type = EModuleTypes::Sight;
}

void AOPAG_SightBase::BeginPlay()
{
	Super::BeginPlay();

	SightCameraComponent->SetComponentTickEnabled(false);
}

void AOPAG_SightBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AOPAG_SightBase::OnAttach(const EModuleRarity NewRarity, const FOPAG_GunStats NewStats,
	const int NewEquippableFireTypes, const bool bIsAttachedToPlayer)
{
	Super::OnAttach(NewRarity, NewStats, NewEquippableFireTypes, bIsAttachedToPlayer);

	SightCameraComponent->SetComponentTickEnabled(true);
}