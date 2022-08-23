#include "Weapons/Modules/OPAG_ModuleBase.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

AOPAG_ModuleBase::AOPAG_ModuleBase()
{
	PrimaryActorTick.bCanEverTick = true;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	if (Sphere)
	{
		Sphere->SetSphereRadius(32.f);
		Sphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
		RootComponent = Sphere;
	}

	ModuleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ModuleMesh"));
	if (ModuleMesh)
	{
		ModuleMesh->SetCollisionProfileName(TEXT("Weapon"));
		ModuleMesh->SetupAttachment(Sphere);
	}
}

FString AOPAG_ModuleBase::InteractString_Implementation()
{
	return "Pick up module [E]";
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

#pragma region GetAndSet
void AOPAG_ModuleBase::SetParameters(const EModuleRarity NewRarity, const FOPAG_GunStats NewStats, const int NewEquippableFireTypes)
{
	Rarity = NewRarity;
	Stats = NewStats;
	EquippableFireTypes = NewEquippableFireTypes;
}

FString AOPAG_ModuleBase::GetRarityName(EModuleRarity ModuleRarity)
{
	FString RarityName;
	UEnum::GetValueAsString(ModuleRarity, RarityName);
	RarityName.RemoveAt(0, 15, true);
	return RarityName;
}

FString AOPAG_ModuleBase::GetTypeName(EModuleTypes ModuleType)
{
	FString TypeName;
	UEnum::GetValueAsString(ModuleType, TypeName);
	TypeName.RemoveAt(0, 14, true);
	return TypeName;
}

UPrimitiveComponent* AOPAG_ModuleBase::GetSphereComponent() const
{
	return Sphere;
}
#pragma endregion GetAndSet

#pragma region Interface
void AOPAG_ModuleBase::Interact_Implementation()
{

}

bool AOPAG_ModuleBase::CanInteract_Implementation()
{
	if (GetParentActor() == nullptr)
	{
		return true;
	}
	return false;
}

void AOPAG_ModuleBase::ApplyForce_Implementation(const FVector Force)
{
	ModuleMesh->SetSimulatePhysics(true);
	ModuleMesh->AddForce(Force, NAME_None, true);
}
#pragma endregion Interface

void AOPAG_ModuleBase::OnAttach(const EModuleRarity NewRarity, const FOPAG_GunStats NewStats, 
	const int NewEquippableFireTypes, const bool bIsAttachedToPlayer)
{
	SetParameters(NewRarity, NewStats, NewEquippableFireTypes);
}

void AOPAG_ModuleBase::CheckIfPhysical() const
{
	// Control if module has parent gun -> is attached
	if (GetParentActor() == nullptr)
	{
		ModuleMesh->SetSimulatePhysics(true);
	}
	else 
	{
		Sphere->SetCollisionProfileName("NoCollision");
		ModuleMesh->SetCollisionProfileName("NoCollision");
	}
}
