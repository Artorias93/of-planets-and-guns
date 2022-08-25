#include "Weapons/Guns/OPAG_ModularGun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ChildActorComponent.h"
#include "Camera/CameraComponent.h"
#include "Weapons/Modules/OPAG_ModuleBase.h"
#include "Weapons/Modules/OPAG_BarrelBase.h"
#include "Weapons/Modules/OPAG_AccessoryBase.h"
#include "Weapons/Modules/OPAG_SightBase.h"
#include "Weapons/Throwables/OPAG_Grenade.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Characters/OPAG_CharacterBase.h"
#include "Util/OPAG_UtilityFunctionLibrary.h"
#include "Characters/Player/OPAG_Player.h"
#include "Managers/OPAG_EventManagerSubsystem.h"
#include "Items/OPAG_Drop.h"
#include "Managers/OPAG_GameInstance.h"
#include <Kismet/GameplayStatics.h>
#include "Interactables/OPAG_Vendor.h"
#include "Characters/Player/OPAG_InventorySystemComponent.h"

AOPAG_ModularGun::AOPAG_ModularGun()
{
	PrimaryActorTick.bCanEverTick = true;

	Barrel = CreateDefaultSubobject<UChildActorComponent>(TEXT("Barrel"));
	Stock = CreateDefaultSubobject<UChildActorComponent>(TEXT("Stock"));
	Grip = CreateDefaultSubobject<UChildActorComponent>(TEXT("Grip"));
	Magazine = CreateDefaultSubobject<UChildActorComponent>(TEXT("Magazine"));
	Sight = CreateDefaultSubobject<UChildActorComponent>(TEXT("Sight"));
	Accessory = CreateDefaultSubobject<UChildActorComponent>(TEXT("Accessory"));
}

const float AOPAG_ModularGun::GetPriceTotal_Implementation()
{
	float PriceTotal = .0f;

	PriceTotal += IOPAG_InteractableInterface::Execute_GetPriceTotal(GetModule(Barrel));
	PriceTotal += IOPAG_InteractableInterface::Execute_GetPriceTotal(GetModule(Stock));
	PriceTotal += IOPAG_InteractableInterface::Execute_GetPriceTotal(GetModule(Grip));
	PriceTotal += IOPAG_InteractableInterface::Execute_GetPriceTotal(GetModule(Magazine));

	if(GetModule(Sight) != nullptr)
		PriceTotal += IOPAG_InteractableInterface::Execute_GetPriceTotal(GetModule(Sight));
	if(GetModule(Accessory) != nullptr)
		PriceTotal += IOPAG_InteractableInterface::Execute_GetPriceTotal(GetModule(Accessory));

	return PriceTotal;
}

bool AOPAG_ModularGun::CanBuy_Implementation()
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

void AOPAG_ModularGun::FlyRotationTimer_Implementation()
{
	// on every frame change rotationg for a smooth rotating actor
	FRotator NewRotation = FRotator(0, 1, 0);
	FQuat QuatRotation = FQuat(NewRotation);
	Receiver->AddWorldRotation(QuatRotation, false, 0, ETeleportType::None);
}

void AOPAG_ModularGun::ApplyFly_Implementation()
{
	Receiver->SetSimulatePhysics(false);
	GetWorld()->GetTimerManager().SetTimer(FlyRotationHandle, this, &AOPAG_ModularGun::FlyRotationTimer_Implementation, .01f, true);
}

FString AOPAG_ModularGun::InteractString_Implementation()
{
	return "Swap gun [E]";
}

void AOPAG_ModularGun::ApplyForce_Implementation(const FVector Force)
{
	Receiver->SetSimulatePhysics(true);
	Receiver->AddForce(Force, NAME_None, true);
}

void AOPAG_ModularGun::Interact_Implementation()
{
}

bool AOPAG_ModularGun::CanInteract_Implementation()
{
	if (GetParentActor() == nullptr)
	{
		return true;
	}

	return false;
}

void AOPAG_ModularGun::BeginPlay()
{
	Super::BeginPlay();

	CreateAndAttachAllModules(GetStandardModulesStruct());
	PopulateAmmoTable();
	CheckIfPhysical();

	AmmoCurrent = Stats.ClipSize;
}

void AOPAG_ModularGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AOPAG_ModularGun::PopulateAmmoTable()
{
	if (AmmoReloadingMultipliersTable)
	{
		const FString Context;
		AmmoReloadingMultipliersTable->GetAllRows(Context, AmmoReloadingMultipliers);
	}
}

void AOPAG_ModularGun::CheckIfPhysical() const
{
	// Control if module has parent gun -> is attached
	if (GetParentActor() == nullptr)
	{
		Receiver->SetSimulatePhysics(true);
	}
}

#pragma region Setter
void AOPAG_ModularGun::SetAmmoParams(int NewAmmoCurrent, int NewAmmoMax)
{
	AmmoCurrent = NewAmmoCurrent;
	AmmoMax = NewAmmoMax;
}
#pragma endregion Setter

#pragma region GET
FOPAG_GunModules AOPAG_ModularGun::GetModulesStruct() const
{
	FOPAG_GunModules GunModules;
	GunModules.Barrel = Barrel->GetChildActorClass();
	GunModules.Stock = Stock->GetChildActorClass();
	GunModules.Grip = Grip->GetChildActorClass();
	GunModules.Magazine = Magazine->GetChildActorClass();
	GunModules.Sight = Sight->GetChildActorClass();
	GunModules.Accessory = Accessory->GetChildActorClass();

	return GunModules;
}

FOPAG_GunModules AOPAG_ModularGun::GetStandardModulesStruct() const
{
	FOPAG_GunModules StandardModules;
	StandardModules.Barrel = StandardBarrel;
	StandardModules.Stock = StandardStock;
	StandardModules.Grip = StandardGrip;
	StandardModules.Magazine = StandardMagazine;

	return StandardModules;
}

// Return module by child actor of socket
AOPAG_ModuleBase* AOPAG_ModularGun::GetModule(const UChildActorComponent* const ChildComponent) const
{
	AOPAG_ModuleBase* Module = Cast<AOPAG_ModuleBase>(ChildComponent->GetChildActor());
	return Module;
}

// Return module by module type
AOPAG_ModuleBase* AOPAG_ModularGun::GetModuleBySocketType(const EModuleTypes ModuleType) const
{
	AOPAG_ModuleBase* Module = nullptr;

	switch (ModuleType)
	{
	case EModuleTypes::Barrel:
		Module = GetModule(Barrel);
		break;
	case EModuleTypes::Stock:
		Module = GetModule(Stock);
		break;
	case EModuleTypes::Grip:
		Module = GetModule(Grip);
		break;
	case EModuleTypes::Magazine:
		Module = GetModule(Magazine);
		break;
	case EModuleTypes::Sight:
		Module = GetModule(Sight);
		break;
	case EModuleTypes::Accessory:
		Module = GetModule(Accessory);
		break;
	}
	return Module;
}

UChildActorComponent* AOPAG_ModularGun::GetSocketByModuleType(const EModuleTypes ModuleType) const
{
	UChildActorComponent* ModuleSlot = nullptr;

	switch (ModuleType)
	{
	case EModuleTypes::Barrel:
		ModuleSlot = Barrel;
		break;
	case EModuleTypes::Stock:
		ModuleSlot = Stock;
		break;
	case EModuleTypes::Grip:
		ModuleSlot = Grip;
		break;
	case EModuleTypes::Magazine:
		ModuleSlot = Magazine;
		break;
	case EModuleTypes::Sight:
		ModuleSlot = Sight;
		break;
	case EModuleTypes::Accessory:
		ModuleSlot = Accessory;
		break;
	}
	return ModuleSlot;
}

// Return socket's name by module type
// Remove the first 14 chars because getvalues return EModuleTypes::X
FName AOPAG_ModularGun::GetSocketNameByModuleType(const EModuleTypes ModuleType) const
{
	FString SocketName;
	UEnum::GetValueAsString(ModuleType, SocketName);
	SocketName.RemoveAt(0, 14, true);
	return FName(*SocketName);
}


void AOPAG_ModularGun::DetachAllModules(const FVector Force)
{
	DetachModule(Barrel, Force);
	DetachModule(Stock, Force);
	DetachModule(Grip, Force);
	DetachModule(Magazine, Force);
	DetachModule(Sight, Force);
	DetachModule(Accessory, Force);
}

void AOPAG_ModularGun::DetachModule(UChildActorComponent* ChildModule, const FVector Force)
{
	if (ChildModule->GetChildActor() == nullptr)
	{
		return;
	}

	const AOPAG_Player* Player = Cast<AOPAG_Player>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (Player == nullptr)
	{
		return;
	}

	// Control check for don't consider the standard module
	AOPAG_ModuleBase* CurrentChildModule = GetModule(ChildModule);
	switch (CurrentChildModule->Type)
	{
		case EModuleTypes::Barrel:
			if (CurrentChildModule->GetClass() == StandardBarrel)
			{
				return;
			}
			break;
		case EModuleTypes::Stock:
			if (CurrentChildModule->GetClass() == StandardStock)
			{
				return;
			}
			break;
		case EModuleTypes::Grip:
			if (CurrentChildModule->GetClass() == StandardGrip)
			{
				return;
			}
			break;
		case EModuleTypes::Magazine:
			if (CurrentChildModule->GetClass() == StandardMagazine)
			{
				return;
			}
			break;
	}

	const TSubclassOf<AOPAG_ModuleBase> TempModuleClass = CurrentChildModule->GetClass();
	const FOPAG_GunStats TempModuleStats = CurrentChildModule->Stats;
	const EModuleRarity TempModuleRarity = CurrentChildModule->Rarity;
	const int TempModuleEquippableFireTypes = CurrentChildModule->EquippableFireTypes;

	// Destroy ChildActor
	ChildModule->DestroyChildActor();
	ChildModule->SetChildActorClass(nullptr);

	// Spawn new module
	const FVector SpawnLocation = Player->GetActorLocation();
	const FRotator SpawnRotator = FRotator::ZeroRotator;
	AActor* TempActor = GetWorld()->SpawnActor(TempModuleClass, &SpawnLocation, &SpawnRotator);
	AOPAG_ModuleBase* TempModule = Cast<AOPAG_ModuleBase>(TempActor);
	TempModule->ApplyForce_Implementation(Force);

	// If the module isn't the standard, so is important attach the standard 
	switch (TempModule->Type)
	{
	case EModuleTypes::Barrel:
		CreateModule(ChildModule, StandardBarrel);
		break;
	case EModuleTypes::Stock:
		CreateModule(ChildModule, StandardStock);
		break;
	case EModuleTypes::Grip:
		CreateModule(ChildModule, StandardGrip);
		break;
	case EModuleTypes::Magazine:
		CreateModule(ChildModule, StandardMagazine);
		break;
	}
}

UCameraComponent* AOPAG_ModularGun::GetSightCamera() const
{
	if (Sight->GetChildActor())
	{
		const AOPAG_SightBase* const Module = Cast<AOPAG_SightBase>(Sight->GetChildActor());
		return Module->GetCamera();
	}
	return nullptr;
}

void AOPAG_ModularGun::GetCurrentState(FOPAG_GunState& GunState) const
{
	GunState.GunClass = GetClass();

	GunState.AttachedModules.Barrel = Barrel->GetChildActorClass();
	GunState.AttachedModules.Stock = Stock->GetChildActorClass();
	GunState.AttachedModules.Grip = Grip->GetChildActorClass();
	GunState.AttachedModules.Sight = Sight->GetChildActorClass();
	GunState.AttachedModules.Accessory = Accessory->GetChildActorClass();

	GunState.AmmoCurrent = AmmoCurrent;
	GunState.AmmoMax = AmmoMax;
}
#pragma endregion GET

#pragma region Shoot Behavior
void AOPAG_ModularGun::ShootThrowable(const FRotator SpawnRotation)
{
	const AOPAG_BarrelBase* const Module = Cast<AOPAG_BarrelBase>(Barrel->GetChildActor());
	if (Module)
	{
		const USceneComponent* const Muzzle = Module->MuzzleComponent;
		if (Muzzle)
		{
			const FVector SpawnLocation = Muzzle->GetComponentLocation() + Muzzle->GetForwardVector()*20;
			SpawnThrowable(SpawnLocation, SpawnRotation);
		}
	}
}

void AOPAG_ModularGun::SpawnThrowable(const FVector SpawnLocation, const FRotator SpawnRotation)
{
	if (Accessory->GetChildActor())
	{
		const AOPAG_AccessoryBase* Module = Cast<AOPAG_AccessoryBase>(Accessory->GetChildActor());
		const TSubclassOf<AOPAG_Grenade> GrenadeClass = Module->Grenade;
		const FActorSpawnParameters SpawnInfo;
		GetWorld()->SpawnActor(GrenadeClass, &SpawnLocation, &SpawnRotation, SpawnInfo);
	}
}

USceneComponent* AOPAG_ModularGun::GetMuzzle() const
{
	const AOPAG_BarrelBase* const Module = Cast<AOPAG_BarrelBase>(Barrel->GetChildActor());
	if (Module)
	{
		if (Module->MuzzleComponent)
			return Module->MuzzleComponent;
	}
	return nullptr;
}
#pragma endregion Shoot Behavior

#pragma region Create and Attach Modules
void AOPAG_ModularGun::CreateAndAttachAllModules(const FOPAG_GunModules& Modules)
{
	if (Modules.Barrel) CreateModule(Barrel, Modules.Barrel);
	if (Modules.Stock) CreateModule(Stock, Modules.Stock);
	if (Modules.Grip) CreateModule(Grip, Modules.Grip);
	if (Modules.Magazine) CreateModule(Magazine, Modules.Magazine);
	if (Modules.Sight) CreateModule(Sight, Modules.Sight);
	if (Modules.Accessory) CreateModule(Accessory, Modules.Accessory);

	// Update Stats
	UpdateStats();
}

// Is important when generate the basic gun 
void AOPAG_ModularGun::CreateModule(UChildActorComponent* ModuleSlot, const TSubclassOf<AOPAG_ModuleBase> Module) const
{
	if (Module)
	{
		const AOPAG_ModuleBase* const Base = Module->GetDefaultObject<AOPAG_ModuleBase>();
		if (Base)
		{
			const EModuleRarity ModuleRarity = Base->Rarity;
			const FOPAG_GunStats ModuleStats = Base->Stats;
			const int ModuleEquippableFireTypes = Base->EquippableFireTypes;

			AttachModule(ModuleSlot, Module, ModuleRarity, ModuleStats, ModuleEquippableFireTypes);
		}
	}
}

// Set child create the object in scene and set the actor's child
// After this is important set the specific params of module (when invoke set child... the module has the default params, not the values in scene)
bool AOPAG_ModularGun::AttachModule(UChildActorComponent* ModuleSlot, const TSubclassOf<AOPAG_ModuleBase> NewModule,
	const EModuleRarity NewModuleRarity, const FOPAG_GunStats NewModuleStats, const int NewModuleEquippableFireTypes) const
{
	ModuleSlot->SetChildActorClass(NewModule);

	AOPAG_ModuleBase* Module = GetModule(ModuleSlot);
	if (Module)
	{
		Module->OnAttach(NewModuleRarity, NewModuleStats, NewModuleEquippableFireTypes, 
			GetParentActor() == GetWorld()->GetFirstPlayerController()->GetPawn());

		const FName SocketName = GetSocketNameByModuleType(Module->Type);
		Module->AttachToComponent(Receiver, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
		return true;
	}
	return false;
}

void AOPAG_ModularGun::UpdateStats()
{
	// Create module's array
	TArray<AOPAG_ModuleBase*> Modules;

	AOPAG_ModuleBase* ModuleBarrel = GetModule(Barrel);
	Modules.Add(ModuleBarrel);
	AOPAG_ModuleBase* ModuleStock = GetModule(Stock);
	Modules.Add(ModuleStock);
	AOPAG_ModuleBase* ModuleGrip = GetModule(Grip);
	Modules.Add(ModuleGrip);
	AOPAG_ModuleBase* ModuleMagazine = GetModule(Magazine);
	Modules.Add(ModuleMagazine);

	if (Sight->GetChildActor())
	{
		AOPAG_ModuleBase* ModuleSight = GetModule(Sight);
		Modules.Add(ModuleSight);
	}
	if (Accessory->GetChildActor())
	{
		AOPAG_ModuleBase* ModuleAccessory = GetModule(Accessory);
		Modules.Add(ModuleAccessory);
	}

	Stats = BaseStats;

	// Update stats
	for (int i = 0; i < Modules.Num(); i++)
	{
		Stats.Damage += Modules[i]->Stats.Damage;
		Stats.FireRate += Modules[i]->Stats.FireRate;
		Stats.Accuracy += Modules[i]->Stats.Accuracy;
		Stats.RecoilRate += Modules[i]->Stats.RecoilRate;
		Stats.ReloadingTime += Modules[i]->Stats.ReloadingTime;
		Stats.ClipSize += Modules[i]->Stats.ClipSize;
	}
}

// When player pick new module in scene, the event call this method 
bool AOPAG_ModularGun::AddNewModule(const TSubclassOf<AOPAG_ModuleBase> NewModule, const EModuleRarity NewModuleRarity, const FOPAG_GunStats NewModuleStats, const int NewModuleEquippableFireTypes, TSubclassOf<AOPAG_ModuleBase>& OldModuleClass, EModuleRarity& OldModuleRarity, FOPAG_GunStats& OldModuleStats, int& OldModuleEquippableFireTypes)
{
	if (NewModule)
	{
		// Initialize Base (CDO of NewModule for default params of class) and ModuleSlot (the module of main socket ex. Barrel)
		const AOPAG_ModuleBase* const Base = NewModule->GetDefaultObject<AOPAG_ModuleBase>();
		UChildActorComponent* const ModuleSlot = GetSocketByModuleType(Base->Type);
		const AOPAG_ModuleBase* const OldModule = GetModuleBySocketType(Base->Type);

		// Set current value of ModuleSlot -> is important when re-spawn the module in scene 
		if (OldModule)
		{
			OldModuleClass = OldModule->GetClass();
			OldModuleRarity = OldModule->Rarity;
			OldModuleStats = OldModule->Stats;
			OldModuleEquippableFireTypes = OldModule->EquippableFireTypes;
		}

		// Call the attach module method for set child actor with socket
		if (ModuleSlot)
		{
			const bool Result = AttachModule(ModuleSlot, NewModule, NewModuleRarity, NewModuleStats, NewModuleEquippableFireTypes);

			if (Base->Type == EModuleTypes::Magazine)
			{
				UpdateAmmo();
			}

			// Update Stats
			UpdateStats();

			return Result;
		}
	}
	return false;
}
#pragma endregion Create and Attach Modules

#pragma region Ammo System
void AOPAG_ModularGun::UpdateAmmo()
{
	const int ClipSize = Stats.ClipSize;
	if (ClipSize < AmmoCurrent)
	{
		AmmoMax += AmmoCurrent - ClipSize;
	}
}

void AOPAG_ModularGun::Reload()
{
	bIsReloading = true;
	GetWorld()->GetTimerManager().SetTimer(ReloadHandle, this, &AOPAG_ModularGun::WaitReloadingTimer, Stats.ReloadingTime, false);
	UGameplayStatics::PlaySound2D(GetWorld(), ReloadSound);
}

void AOPAG_ModularGun::IncrementAmmo()
{
	for (int i = 0; i < AmmoReloadingMultipliers.Num(); i++)
	{
		if (AmmoReloadingMultipliers[i]->FireType == FireType)
		{
			AmmoMax += AmmoReloadingMultipliers[i]->ReloadingFactor;
			break;
		}
	}

	EMS->TotalAmmoChanged.Broadcast(AmmoMax);

}

// When player reload the gun is impossible to shoot and reload, because of this player will have to wait this timer
void AOPAG_ModularGun::WaitReloadingTimer()
{
	bIsReloading = false;

	const AOPAG_Player* const Player = Cast<AOPAG_Player>(this->GetParentActor());

	const int BulletDifference = Stats.ClipSize - AmmoCurrent;
	if (BulletDifference < AmmoMax)
	{
		AmmoCurrent += BulletDifference;
		AmmoMax -= BulletDifference;

		if (Player) {
			Player->EMS->TotalAmmoChanged.Broadcast(AmmoMax);
			Player->EMS->CurrentAmmoChanged.Broadcast(AmmoCurrent);
		}
	}
	else
	{
		AmmoCurrent += AmmoMax;
		AmmoMax = 0;

		if (Player) {
			Player->EMS->TotalAmmoChanged.Broadcast(AmmoMax);
			Player->EMS->CurrentAmmoChanged.Broadcast(AmmoCurrent);
		}
	}
}
#pragma endregion Ammo System