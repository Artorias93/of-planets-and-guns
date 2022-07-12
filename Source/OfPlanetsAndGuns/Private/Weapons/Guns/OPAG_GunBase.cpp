#include "Weapons/Guns/OPAG_GunBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ChildActorComponent.h"
#include "Camera/CameraComponent.h"
#include "Weapons/Modules/OPAG_ModuleBase.h"

AOPAG_GunBase::AOPAG_GunBase()
{
	PrimaryActorTick.bCanEverTick = true;

	Receiver = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Receiver"));
	Receiver->CanCharacterStepUpOn = ECB_Owner;
	Receiver->SetCollisionProfileName("Weapon");
	RootComponent = Receiver;

	ADSCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ADSCamera"));
	if (ADSCamera)
	{
		ADSCamera->SetRelativeLocation(FVector(-0.000046f, -0.454329f, 22.030514f));
		ADSCamera->SetRelativeRotation(FRotator(0, 90.f, 0));
		ADSCamera->SetRelativeScale3D(FVector(.2f, .2f, .2f));
		ADSCamera->SetProjectionMode(ECameraProjectionMode::Perspective);
		ADSCamera->SetFieldOfView(60.f);
	}

	Barrel = CreateDefaultSubobject<UChildActorComponent>(TEXT("Barrel"));
	Stock = CreateDefaultSubobject<UChildActorComponent>(TEXT("Stock"));
	Grip = CreateDefaultSubobject<UChildActorComponent>(TEXT("Grip"));
	Magazine = CreateDefaultSubobject<UChildActorComponent>(TEXT("Magazine"));
	Sight = CreateDefaultSubobject<UChildActorComponent>(TEXT("Sight"));
	Accessory = CreateDefaultSubobject<UChildActorComponent>(TEXT("Accessory"));

	// Barrel Setting
	static ConstructorHelpers::FClassFinder<AActor> BlueprintStandardBarrel(TEXT("/Game/OfPlanetsAndGuns/Weapons/Modules/SpecializedModules/Barrell/BP_StandardBarrell"));
	if (BlueprintStandardBarrel.Succeeded())
	{
		StandardBarrel = BlueprintStandardBarrel.Class;
	}

	// Stock Setting
	static ConstructorHelpers::FClassFinder<AActor> BlueprintStandardStock(TEXT("/Game/OfPlanetsAndGuns/Weapons/Modules/SpecializedModules/Stock/BP_StandardStock"));
	if (BlueprintStandardStock.Succeeded())
	{
		StandardStock = BlueprintStandardStock.Class;
	}

	// Grip Setting
	static ConstructorHelpers::FClassFinder<AActor> BlueprintStandardGrip(TEXT("/Game/OfPlanetsAndGuns/Weapons/Modules/SpecializedModules/Grip/BP_StandardGrip"));
	if (BlueprintStandardGrip.Succeeded())
	{
		StandardGrip = BlueprintStandardGrip.Class;
	}

	// Magazine Setting
	static ConstructorHelpers::FClassFinder<AActor> BlueprintStandardMagazine(TEXT("/Game/OfPlanetsAndGuns/Weapons/Modules/SpecializedModules/Magazine/BP_StandardMagazine"));
	if (BlueprintStandardMagazine.Succeeded())
	{
		StandardMagazine = BlueprintStandardMagazine.Class;
	}
}

void AOPAG_GunBase::BeginPlay()
{
	Super::BeginPlay();
	
	CreateAndAttachAllModules();
}

void AOPAG_GunBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AOPAG_GunBase::CreateAndAttachAllModules()
{
	CreateModule(Barrel, StandardBarrel);
	CreateModule(Stock, StandardStock);
	CreateModule(Grip, StandardGrip);
	CreateModule(Magazine, StandardMagazine);
}

void AOPAG_GunBase::CreateModule(UChildActorComponent* ModuleSlot, const TSubclassOf<AOPAG_ModuleBase> Module)
{
	if (Module)
	{
		AOPAG_ModuleBase* Base = Module->GetDefaultObject<AOPAG_ModuleBase>();
		if (Base)
		{
			const EModuleRarity ModuleRarity = Base->Rarity;
			const FGunStats ModuleStats = Base->Stats;
			const int ModuleEquippableFireTypes = Base->EquippableFireTypes;

			AttachModule(ModuleSlot, Module, ModuleRarity, ModuleStats, ModuleEquippableFireTypes);
		}
	}
}

const bool AOPAG_GunBase::AttachModule(UChildActorComponent* ModuleSlot, const TSubclassOf<AOPAG_ModuleBase> NewModule, const EModuleRarity NewModuleRarity, const FGunStats NewModuleStats, const int NewModuleEquippableFireTypes)
{
	ModuleSlot->SetChildActorClass(NewModule);

	AOPAG_ModuleBase* Module = GetModule(ModuleSlot);
	if (Module)
	{
		Module->OnAttach(NewModuleRarity, NewModuleStats, NewModuleEquippableFireTypes);

		const FName SocketName = GetSocketNameByModuleType(Module->Type);
		Module->AttachToComponent(Receiver, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
		return true;
	}
	return false;
}

AOPAG_ModuleBase* AOPAG_GunBase::GetModule(UChildActorComponent* ChildeComponent)
{
	AOPAG_ModuleBase* Module = Cast<AOPAG_ModuleBase>(ChildeComponent->GetChildActor());
	return Module;
}

const FName AOPAG_GunBase::GetSocketNameByModuleType(const EModuleTypes ModuleType)
{
	FString SocketName;
	UEnum::GetValueAsString(ModuleType, SocketName);
	SocketName.RemoveAt(0, 14, true);
	return FName(*SocketName);
}

void AOPAG_GunBase::Shoot(FVector StartShootLocation, FVector Direction)
{
	ShootLineTrace(StartShootLocation, Direction);

}

void AOPAG_GunBase::ShootLineTrace(FVector StartShootLocation, FVector Direction)
{

}