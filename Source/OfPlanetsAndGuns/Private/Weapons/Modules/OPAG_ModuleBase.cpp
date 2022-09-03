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

	ModuleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ModuleMesh"));
	if (ModuleMesh)
	{
		ModuleMesh->SetCollisionProfileName(TEXT("Weapon"));
		RootComponent = ModuleMesh;
	}

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	if (Sphere)
	{
		Sphere->SetSphereRadius(32.f);
		Sphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
		Sphere->SetupAttachment(ModuleMesh);
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

#pragma region Get
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

FString AOPAG_ModuleBase::GetModuleName() const
{
	return Name;
}

int AOPAG_ModuleBase::GetLevel() const
{
	return Level;
}

EModuleTypes AOPAG_ModuleBase::GetType() const
{
	return Type;
}

EModuleRarity AOPAG_ModuleBase::GetRarity() const
{
	return Rarity;
}

EModuleBrand AOPAG_ModuleBase::GetBrand() const
{
	return Brand;
}

FOPAG_GunStats AOPAG_ModuleBase::GetStats() const
{
	return Stats;
}

int AOPAG_ModuleBase::GetEquippableFireTypes() const
{
	return EquippableFireTypes;
}
#pragma endregion Get

#pragma region Set
void AOPAG_ModuleBase::SetParameters(const EModuleRarity NewRarity, const EModuleBrand NewBrand, const FOPAG_GunStats NewStats, const int NewEquippableFireTypes)
{
	Rarity = NewRarity;
	Brand = NewBrand;
	Stats = NewStats;
	EquippableFireTypes = NewEquippableFireTypes;
}

void AOPAG_ModuleBase::SetStats(const TArray<FOPAG_SecondaryStatModifier*>& StatModifiers)
{
	for (FOPAG_SecondaryStatModifier* Modifier : StatModifiers)
	{
		if (Modifier->Brand == Brand)
		{
			const float BonusDelta = FMath::FRandRange(Level * Modifier->MinValue, Level * Modifier->MaxValue);

			switch (Modifier->StatType)
			{
			case EModuleStatType::Damage:
				if (Stats.Damage == 0.f)
					Stats.Damage += FMath::RoundToInt(BonusDelta);
				break;
			case EModuleStatType::FireRate:
				if (Stats.FireRate == 0.f)
					Stats.FireRate += FMath::RoundToInt(BonusDelta);
				break;
			case EModuleStatType::Accuracy:
				if (Stats.Accuracy == 0.f)
					Stats.Accuracy += FMath::RoundToInt(BonusDelta);
				break;
			case EModuleStatType::RecoilRate:
				if (Stats.RecoilRate == 0.f)
					Stats.RecoilRate += FMath::RoundToInt(BonusDelta);
				break;
			case EModuleStatType::ReloadingTime:
				if (Stats.ReloadingTime == 0.f)
					Stats.ReloadingTime += BonusDelta;
				break;
			case EModuleStatType::ClipSize:
				if (Stats.ClipSize == 0.f)
					Stats.ClipSize += FMath::RoundToInt(BonusDelta);
				break;
			default:;
			}
		}
	}
}

void AOPAG_ModuleBase::SetStats(const FOPAG_GunStats NewStats)
{
	Stats = NewStats;
}
#pragma endregion Set

#pragma region Interface
FString AOPAG_ModuleBase::InteractString_Implementation()
{
	return "Pick up module [E]";
}

void AOPAG_ModuleBase::Interact_Implementation()
{

}

bool AOPAG_ModuleBase::CanInteract_Implementation()
{
	if (GetParentActor() == nullptr)
		return true;

	if (AOPAG_Vendor* vendor = Cast<AOPAG_Vendor>(GetParentActor()))
		return true;

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
	return PriceBase * (1 + PriceMuliplierPercentage * (Level - 1));
}

bool AOPAG_ModuleBase::CanBuy_Implementation()
{
	AOPAG_Player* Player = Cast<AOPAG_Player>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (Player == nullptr)
	{
		return false;
	}

	// Equippable fire type check
	const int FireMask = 1 << (int)Player->GetPrimaryGun()->GetFireType();

	AOPAG_Vendor* Vendor = Cast<AOPAG_Vendor>(GetParentActor());
	if (Vendor != nullptr && FireMask & EquippableFireTypes && Player->GetInventorySystemComponent()->GoldAmount >= IOPAG_InteractableInterface::Execute_GetPriceTotal(this))
	{
		return true;
	}
	return false;
}
#pragma endregion Interface

#pragma region Attachment
void AOPAG_ModuleBase::OnAttach(const EModuleRarity NewRarity, const EModuleBrand NewBrand,const FOPAG_GunStats NewStats,
	const int NewEquippableFireTypes, const bool bIsAttachedToPlayer)
{
	SetParameters(NewRarity, NewBrand, NewStats, NewEquippableFireTypes);
}

void AOPAG_ModuleBase::OnDetach()
{

}

#pragma endregion Attachment

#pragma region Physics
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
#pragma endregion Physics
