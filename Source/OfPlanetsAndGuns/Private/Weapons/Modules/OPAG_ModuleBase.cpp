#include "Weapons/Modules/OPAG_ModuleBase.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Interactables/OPAG_Vendor.h"
#include "Characters/Player/OPAG_Player.h"
#include <Kismet/GameplayStatics.h>
#include "Characters/Player/OPAG_InventorySystemComponent.h"
#include "Curves/CurveVector.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "EngineUtils.h"

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

	LoadDropParticleAssets();
}

void AOPAG_ModuleBase::BeginPlay()
{
	Super::BeginPlay();	

	ModuleDropHighlight->Deactivate();
	ModuleDropHighlight->bAutoActivate = false;

	// This method is call for activate physical behavior when module is in the scene
	CheckIfPhysical();

	Sphere->SetSimulatePhysics(false);

	InitDropTimeline();

	SelectParticleFromRarity(GetRarity());

	if (ModuleDropHighlightParticleInUse != nullptr)
	{
		ModuleDropHighlight->SetTemplate(ModuleDropHighlightParticleInUse);
		ModuleDropHighlight->SetWorldScale3D(FVector(0.1f, 0.1f, 0.1f));		
	}

	if (GetParentActor() == nullptr)
	{
		ModuleMesh->SetWorldScale3D(FVector(2.f, 2.f, 2.f));
		GetWorld()->GetTimerManager().SetTimer(FlyRotationHandle, this, &AOPAG_ModuleBase::FlyRotationTimer_Implementation, .01f, true);
	}
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
	if (Level == 2)
	{
		return EModuleRarity::Uncommon;
	}
	if (Level == 3)
	{
		return EModuleRarity::Rare;
	}
	if (Level == 4)
	{
		return EModuleRarity::Epic;
	}
	if (Level >= 5)
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
void AOPAG_ModuleBase::SelectParticleFromRarity(EModuleRarity ModuleRarity)
{
	switch (ModuleRarity)
	{
	case EModuleRarity::Common:
		ModuleDropHighlightParticleInUse = ModuleDropHighlightParticleCommon;
		break;

	case EModuleRarity::Uncommon:
		ModuleDropHighlightParticleInUse = ModuleDropHighlightParticleUncommon;
		break;

	case EModuleRarity::Rare:
		ModuleDropHighlightParticleInUse = ModuleDropHighlightParticleRare;
		break;

	case EModuleRarity::Epic:
		ModuleDropHighlightParticleInUse = ModuleDropHighlightParticleEpic;
		break;

	case EModuleRarity::Legendary:
		ModuleDropHighlightParticleInUse = ModuleDropHighlightParticleLegendary;
		break;

	default:
		ModuleDropHighlightParticleInUse = nullptr;
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
	
}

void AOPAG_ModuleBase::ApplyFly_Implementation()
{
	ModuleMesh->SetSimulatePhysics(false);
	GetWorld()->GetTimerManager().SetTimer(FlyRotationHandle, this, &AOPAG_ModuleBase::FlyRotationTimer_Implementation, .01f, true);
}

void AOPAG_ModuleBase::FlyRotationTimer_Implementation()
{
	// on every frame change rotating for a smooth rotating actor
	FRotator NewRotation = FRotator(0, 1, 0);
	FQuat QuatRotation = FQuat(NewRotation);
	ModuleMesh->AddWorldRotation(QuatRotation, false, 0, ETeleportType::None);
}

const float AOPAG_ModuleBase::GetPriceTotal_Implementation()
{
	return 20 * Level; // Temporary
	// return PriceBase * (1 + PriceMuliplierPercentage * (Level - 1));
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
void AOPAG_ModuleBase::StartDropTimeline()
{
	SpawnLocation = GetActorLocation();

	// Find a random angle in order to determine the direction where the module will drop
	float RandomAngle = FMath::DegreesToRadians(FMath::RandRange(0, 360));
	RandomDropDir.X = FMath::Sin(RandomAngle);
	RandomDropDir.Y = FMath::Cos(RandomAngle);

	ModuleDropTimeline->PlayFromStart();
}

void AOPAG_ModuleBase::ActivateHighlightParticle()
{
	if (GetParentActor() == nullptr)
	{
		ModuleDropHighlight->Activate();
		ModuleDropHighlight->SetVisibility(true);
		ModuleDropHighlight->bAutoActivate = true;		
	}
}

void AOPAG_ModuleBase::ModuleDropTimelineFinished()
{
	ActivateHighlightParticle();
}

void AOPAG_ModuleBase::InitDropTimeline()
{
	ModuleDropTimeline->SetLooping(false);

	OnModuleDropTimelineProgress.BindUFunction(this, FName("ModuleDropTimelineProgress"));
	ModuleDropTimeline->AddInterpVector(CurveModuleDropVector, OnModuleDropTimelineProgress, FName("ModuleDropTimelinePropertyName"));

	OnModuleDropTimelineFinished.BindUFunction(this, FName("ModuleDropTimelineFinished"));
	ModuleDropTimeline->SetTimelineFinishedFunc(OnModuleDropTimelineFinished);
}
void AOPAG_ModuleBase::LoadDropParticleAssets()
{
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleEffectCommon(TEXT("'/Game/OfPlanetsAndGuns/Effects/Modules/P_Modules_common_v2.P_Modules_common_v2'"));
	if (ParticleEffectCommon.Succeeded())
	{
		ModuleDropHighlightParticleCommon = ParticleEffectCommon.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleEffectUncommon(TEXT("'/Game/OfPlanetsAndGuns/Effects/Modules/P_Modules_uncommon_v2.P_Modules_uncommon_v2'"));
	if (ParticleEffectUncommon.Succeeded())
	{
		ModuleDropHighlightParticleUncommon = ParticleEffectUncommon.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleEffectRare(TEXT("'/Game/OfPlanetsAndGuns/Effects/Modules/P_Modules_rare_v2.P_Modules_rare_v2'"));
	if (ParticleEffectRare.Succeeded())
	{
		ModuleDropHighlightParticleRare = ParticleEffectRare.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleEffectEpic(TEXT("'/Game/OfPlanetsAndGuns/Effects/Modules/P_Modules_epic_v2.P_Modules_epic_v2'"));
	if (ParticleEffectEpic.Succeeded())
	{
		ModuleDropHighlightParticleEpic = ParticleEffectEpic.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleEffectLegendary(TEXT("'/Game/OfPlanetsAndGuns/Effects/Modules/P_Modules_legendary_v2.P_Modules_legendary_v2'"));
	if (ParticleEffectLegendary.Succeeded())
	{
		ModuleDropHighlightParticleLegendary = ParticleEffectLegendary.Object;
	}
}
#pragma endregion Drop