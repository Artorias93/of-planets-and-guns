#include "Weapons/Modules/OPAG_ModuleBase.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Interactables/OPAG_Vendor.h"
#include "Characters/Player/OPAG_Player.h"
#include <Kismet/GameplayStatics.h>
#include "Characters/Player/OPAG_InventorySystemComponent.h"
#include "Curves/CurveVector.h"
#include "Particles/ParticleSystem.h"
#include "Components/PointLightComponent.h"
#include "Particles/ParticleSystemComponent.h"

AOPAG_ModuleBase::AOPAG_ModuleBase()
{
	PrimaryActorTick.bCanEverTick = true;

	ModuleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ModuleMesh"));
	if (ModuleMesh)
	{
		ModuleMesh->SetCollisionProfileName(TEXT("Weapon"));
		RootComponent = ModuleMesh;
		ModuleMesh->SetSimulatePhysics(false);
	}

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	if (Sphere)
	{
		Sphere->SetSphereRadius(32.f);
		Sphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
		Sphere->SetupAttachment(ModuleMesh);
		Sphere->SetSimulatePhysics(false);
	}

	ModuleDropHighlight = CreateDefaultSubobject<UParticleSystemComponent>("HightLight Particle");
	{
		ModuleDropHighlight->SetupAttachment(Sphere);
	}

	ModuleDropTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("ModuleDropTimeline"));
	
	static ConstructorHelpers::FObjectFinder<UCurveVector> CurveVector (TEXT("'/Game/OfPlanetsAndGuns/Interactables/Curve_Drop_Vector.Curve_Drop_Vector'"));
	if (CurveVector.Succeeded())
	{
		CurveModuleDropVector = CurveVector.Object;
	}
}

void AOPAG_ModuleBase::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnLocation = GetActorLocation();

	ModuleDropHighlight->Deactivate();
	ModuleDropHighlight->bAutoActivate = false;

	// This method is call for activate physical behavior when module is in the scene
	CheckIfPhysical();
	Sphere->SetSimulatePhysics(false);
	
	ModuleDropTimeline->SetLooping(false);
	ModuleDropTimeline->SetTimelineLengthMode(ETimelineLengthMode::TL_TimelineLength);

	OnModuleDropTimelineProgress.BindUFunction(this, FName("ModuleDropTimelineProgress"));
	ModuleDropTimeline->AddInterpVector(CurveModuleDropVector, OnModuleDropTimelineProgress, FName("ModuleDropTimelinePropertyName"));

	// Find a random angle in order to determine the direction where the module will drop
	float RandomAngle = FMath::DegreesToRadians(FMath::RandRange(0, 360));
	RandomDropDir.X = FMath::Sin(RandomAngle);
	RandomDropDir.Y = FMath::Cos(RandomAngle);

	LoadParticleFromRarity(GetRarity());

	if (ModuleDropHighlightParticle != nullptr)
	{
		ModuleDropHighlight->SetTemplate(ModuleDropHighlightParticle);

		if (GetParentActor() == nullptr)
		{
			ModuleDropHighlight->Activate();
			ModuleDropHighlight->SetVisibility(true);
			ModuleDropHighlight->bAutoActivate = true;

			ModuleMesh->SetWorldScale3D(FVector(2, 2, 2));

			ModuleDropTimeline->PlayFromStart();
		}
	}
}

void AOPAG_ModuleBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Need to disable the physics into the Tick to make it apply effectively, probably it's set true after begin play when droped on enemy killed
	Sphere->SetSimulatePhysics(false);

	// animation to let light grow and intensify
	if (GetParentActor() == nullptr && CurrentHighlightIncreaseSizeFactor < HighlightIncreaseSizeFactor)
	{
		CurrentHighlightIncreaseSizeFactor += DeltaTime;		
	}
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
	if (Level > 11 && Level < 21)
	{
		return EModuleRarity::Uncommon;
	}
	if (Level > 21 && Level < 31)
	{
		return EModuleRarity::Rare;
	}
	if (Level > 31 && Level < 41)
	{
		return EModuleRarity::Epic;
	}
	if (Level > 41)
	{
		return EModuleRarity::Legendary;
	}

	return EModuleRarity::Common;
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
void AOPAG_ModuleBase::LoadParticleFromRarity(EModuleRarity ModuleRarity)
{

	switch (ModuleRarity)
	{
	case EModuleRarity::Common:
		ModuleDropHighlightParticle = LoadObject<UParticleSystem>(NULL, TEXT("'/Game/OfPlanetsAndGuns/Effects/Modules/P_Modules_Common.P_Modules_Common'"), NULL, LOAD_None, NULL);
		break;

	case EModuleRarity::Uncommon:
		ModuleDropHighlightParticle = LoadObject<UParticleSystem>(NULL, TEXT("'/Game/OfPlanetsAndGuns/Effects/Modules/P_Modules_Uncommon.P_Modules_Uncommon'"), NULL, LOAD_None, NULL);
		break;

	case EModuleRarity::Rare:
		ModuleDropHighlightParticle = LoadObject<UParticleSystem>(NULL, TEXT("'/Game/OfPlanetsAndGuns/Effects/Modules/P_Modules_Rare.P_Modules_Rare'"), NULL, LOAD_None, NULL);
		break;

	case EModuleRarity::Epic:
		ModuleDropHighlightParticle = LoadObject<UParticleSystem>(NULL, TEXT("'/Game/OfPlanetsAndGuns/Effects/Modules/P_Modules_Epic.P_Modules_Epic'"), NULL, LOAD_None, NULL);
		break;

	case EModuleRarity::Legendary:
		ModuleDropHighlightParticle = LoadObject<UParticleSystem>(NULL, TEXT("'/Game/OfPlanetsAndGuns/Effects/Modules/P_Modules_Legendary.P_Modules_Legendary'"), NULL, LOAD_None, NULL);
		break;
	}
}
#pragma endregion Get

#pragma region Set
void AOPAG_ModuleBase::SetParameters(const EModuleBrand NewBrand, const FOPAG_GunStats NewStats, const int NewEquippableFireTypes)
{
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
			case EModuleStatType::Stability:
				if (Stats.Stability == 0.f)
					Stats.Stability += FMath::RoundToInt(BonusDelta);
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

	return false;
}

void AOPAG_ModuleBase::ApplyForce_Implementation(const FVector Force)
{
	//ModuleMesh->SetSimulatePhysics(true);
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
void AOPAG_ModuleBase::OnAttach(const EModuleBrand NewBrand,const FOPAG_GunStats NewStats,
	const int NewEquippableFireTypes, const bool bIsAttachedToPlayer)
{
	// when it's attached to the gun, scale it to original dimension
	ModuleMesh->SetWorldScale3D(FVector(1, 1, 1));

	SetParameters(NewBrand, NewStats, NewEquippableFireTypes);
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
		//ModuleMesh->SetSimulatePhysics(true);
	}
	else
	{
		Sphere->SetCollisionProfileName("NoCollision");
		ModuleMesh->SetCollisionProfileName("NoCollision");
	}
}
#pragma endregion Physics

#pragma region Drop
void AOPAG_ModuleBase::ModuleDropTimelineProgress(FVector Value)
{
	FVector NewLocation = SpawnLocation;
	NewLocation.X += RandomDropDir.X * Value.X * ModuleDropCurveXFactor;
	NewLocation.Y += RandomDropDir.Y * Value.X * ModuleDropCurveXFactor;
	NewLocation.Z += Value.Z * ModuleDropCurveZFactor;

	SetActorLocation(NewLocation);
}
#pragma endregion Drop