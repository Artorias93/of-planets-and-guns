#include "Weapons/Modules/OPAG_SightBase.h"
#include "Camera/CameraComponent.h"

AOPAG_SightBase::AOPAG_SightBase() : Super::AOPAG_ModuleBase()
{
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SightCamera"));
	CameraComponent->SetRelativeLocation(FVector(-0.000095f, -18.851955f, 6.571584f));
	CameraComponent->SetRelativeRotation(FRotator(0, 90.f, 0));
	CameraComponent->SetRelativeScale3D(FVector(.2f, .2f, .2f));
	CameraComponent->SetFieldOfView(50.f);
	CameraComponent->SetupAttachment(RootComponent);

	// Initialize Parameters
	Rarity = EModuleRarity::Common;
	Type = EModuleTypes::Sight;
}

void AOPAG_SightBase::BeginPlay()
{
	Super::BeginPlay();

	CameraComponent->SetComponentTickEnabled(false);
}

void AOPAG_SightBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AOPAG_SightBase::OnAttach(const EModuleRarity NewRarity, const FGunStats NewStats, const int NewEquippableFireTypes)
{
	Super::OnAttach(NewRarity, NewStats, NewEquippableFireTypes);

	CameraComponent->SetComponentTickEnabled(true);
}

UCameraComponent* AOPAG_SightBase::GetCamera() const
{
	return CameraComponent;
}