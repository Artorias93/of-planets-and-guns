#include "Weapons/Modules/OPAG_ModuleBase.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

AOPAG_ModuleBase::AOPAG_ModuleBase()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SphereComponent->SetSphereRadius(32.f);
	SphereComponent->SetCollisionProfileName("Weapon");
	RootComponent = SphereComponent;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ModuleMesh"));
	Mesh->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));
	Mesh->SetCollisionProfileName("NoCollision");
	Mesh->SetupAttachment(RootComponent);
}

void AOPAG_ModuleBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AOPAG_ModuleBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AOPAG_ModuleBase::OnAttach(EModuleRarity NewRarity)
{
	Rarity = NewRarity;
}