#include "Weapons/Modules/OPAG_ModuleBase.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

AOPAG_ModuleBase::AOPAG_ModuleBase()
{
	PrimaryActorTick.bCanEverTick = true;

	// Temporary flag for pick module. When create interface for pickupable object.
	Tags.Add("Interactable");

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetSphereRadius(32.f);
	Sphere->SetCollisionProfileName("Weapon");
	RootComponent = Sphere;

	ModuleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ModuleMesh"));
	if (ModuleMesh)
	{
		ModuleMesh->SetCollisionProfileName("NoCollision");
		ModuleMesh->SetupAttachment(Sphere);
	}
}

void AOPAG_ModuleBase::BeginPlay()
{
	Super::BeginPlay();
	
	// This method is call for activate physical behavior when module is in the scene
	CheckIfPhysical();
}

void AOPAG_ModuleBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AOPAG_ModuleBase::OnAttach(const EModuleRarity NewRarity, const FOPAG_GunStats NewStats, const int NewEquippableFireTypes)
{
	SetParameters(NewRarity, NewStats, NewEquippableFireTypes);
}

void AOPAG_ModuleBase::SetParameters(const EModuleRarity NewRarity, const FOPAG_GunStats NewStats, const int NewEquippableFireTypes)
{
	Rarity = NewRarity;
	Stats = NewStats;
	EquippableFireTypes = NewEquippableFireTypes;
}

UPrimitiveComponent* AOPAG_ModuleBase::GetSphereComponent() const
{
	return Sphere;
}

void AOPAG_ModuleBase::CheckIfPhysical()
{
	// Control if module has parent gun -> is attached
	if (GetParentActor() == nullptr)
	{
		Sphere->SetSimulatePhysics(true);
	}
}