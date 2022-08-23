#include "Weapons/Modules/OPAG_ModuleBase.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Interactables/OPAG_Vendor.h"
#include "Characters/Player/OPAG_Player.h"
#include <Kismet/GameplayStatics.h>
#include "Characters/Player/OPAG_InventorySystemComponent.h"

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

void AOPAG_ModuleBase::ApplyFly_Implementation()
{
	ModuleMesh->SetSimulatePhysics(false);
	GetWorld()->GetTimerManager().SetTimer(FlyRotationHandle, this, &AOPAG_ModuleBase::FlyRotationTimer_Implementation, .01f, true);
}

void AOPAG_ModuleBase::FlyRotationTimer_Implementation()
{
	// on every frame change rotationg for a smooth rotating actor
	FRotator NewRotation = FRotator(0, 1, 0);
	FQuat QuatRotation = FQuat(NewRotation);
	ModuleMesh->AddWorldRotation(QuatRotation, false, 0, ETeleportType::None);
}

const float AOPAG_ModuleBase::GetPriceTotal_Implementation()
{
	return PriceBase * (1 + (Level - 1));
}

bool AOPAG_ModuleBase::CanBuy_Implementation()
{
	AOPAG_Player* Player = Cast<AOPAG_Player>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (Player == nullptr)
	{
		return false;
	}

	AOPAG_Vendor* Vendor = Cast<AOPAG_Vendor>(GetParentActor());
	if (Vendor != nullptr && Player->InventorySystemComp->GoldAmount >= IOPAG_InteractableInterface::Execute_GetPriceTotal(this))
	{
		return true;
	}
	return false;
}
#pragma endregion Interface

void AOPAG_ModuleBase::OnAttach(const EModuleRarity NewRarity, const FOPAG_GunStats NewStats, 
	const int NewEquippableFireTypes, const bool bIsAttachedToPlayer)
{
	SetParameters(NewRarity, NewStats, NewEquippableFireTypes);
}

void AOPAG_ModuleBase::CheckIfPhysical() const
{
	AOPAG_Vendor* Vendor = Cast<AOPAG_Vendor>(GetParentActor());

	// Control if module has parent gun -> is attached
	if (GetParentActor() == nullptr || Vendor != nullptr)
	{
		ModuleMesh->SetSimulatePhysics(true);
	}
	else 
	{
		Sphere->SetCollisionProfileName("NoCollision");
		ModuleMesh->SetCollisionProfileName("NoCollision");
	}
}
