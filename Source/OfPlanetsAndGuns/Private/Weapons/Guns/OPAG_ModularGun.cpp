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

AOPAG_ModularGun::AOPAG_ModularGun()
{
	PrimaryActorTick.bCanEverTick = true;

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

void AOPAG_ModularGun::BeginPlay()
{
	Super::BeginPlay();

	CreateAndAttachAllModules();
	PopulateAmmoTable();
}

void AOPAG_ModularGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AOPAG_ModularGun::PopulateAmmoTable()
{
	const FString Context;
	if (AmmoReloadingMultipliersTable)
	{
		AmmoReloadingMultipliersTable->GetAllRows(Context, AmmoReloadingMultipliers);
	}
}

#pragma region GET
// Return module by child actor of socket
AOPAG_ModuleBase* AOPAG_ModularGun::GetModule(UChildActorComponent* ChildeComponent) const
{
	AOPAG_ModuleBase* Module = Cast<AOPAG_ModuleBase>(ChildeComponent->GetChildActor());
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
const FName AOPAG_ModularGun::GetSocketNameByModuleType(const EModuleTypes ModuleType) const
{
	FString SocketName;
	UEnum::GetValueAsString(ModuleType, SocketName);
	SocketName.RemoveAt(0, 14, true);
	return FName(*SocketName);
}


UCameraComponent* AOPAG_ModularGun::GetSightCamera()
{
	if (Sight->GetChildActor())
	{
		AOPAG_SightBase* Module = Cast<AOPAG_SightBase>(Sight->GetChildActor());
		return Module->GetCamera();
	}
	return nullptr;
}
#pragma endregion GET

#pragma region Shoot Behavior
void AOPAG_ModularGun::ShootThrowable(const FRotator SpawnRotation)
{
	AOPAG_BarrelBase* Module = Cast<AOPAG_BarrelBase>(Barrel->GetChildActor());
	if (Module)
	{
		UChildActorComponent* Muzzle = Module->MuzzleComponent;
		if (Muzzle)
		{
			const FVector SpawnLocation = Muzzle->GetComponentLocation();
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
#pragma endregion Shoot Behavior

#pragma region Shoot Utility
TArray<AActor*> AOPAG_ModularGun::CheckWhatActorsIgnoreLinetrace()
{
	TArray<AActor*> Objects;
	Objects.Add(GetParentActor());
	Objects.Add(this);
	Objects.Add(Barrel->GetChildActor());
	Objects.Add(Stock->GetChildActor());
	Objects.Add(Grip->GetChildActor());
	Objects.Add(Magazine->GetChildActor());

	// Unique control are for sight and accessory because the gun exist without this two elements
	if (Sight->GetChildActor())
	{
		Objects.Add(Sight->GetChildActor());
	}
	if (Accessory->GetChildActor())
	{
		Objects.Add(Accessory->GetChildActor());
	}
	return Objects;
}
#pragma endregion Shoot Utility

#pragma region Create and Attach Modules
void AOPAG_ModularGun::CreateAndAttachAllModules()
{
	CreateModule(Barrel, StandardBarrel);
	CreateModule(Stock, StandardStock);
	CreateModule(Grip, StandardGrip);
	CreateModule(Magazine, StandardMagazine);

	// Update Stats
	UpdateStats();
}

// Is important when generate the basic gun 
void AOPAG_ModularGun::CreateModule(UChildActorComponent* ModuleSlot, const TSubclassOf<AOPAG_ModuleBase> Module)
{
	if (Module)
	{
		AOPAG_ModuleBase* Base = Module->GetDefaultObject<AOPAG_ModuleBase>();
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
const bool AOPAG_ModularGun::AttachModule(UChildActorComponent* ModuleSlot, const TSubclassOf<AOPAG_ModuleBase> NewModule, const EModuleRarity NewModuleRarity, const FOPAG_GunStats NewModuleStats, const int NewModuleEquippableFireTypes)
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

	Stats = FOPAG_GunStats();

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
const bool AOPAG_ModularGun::AddNewModule(const TSubclassOf<AOPAG_ModuleBase> NewModule, const EModuleRarity NewModuleRarity, const FOPAG_GunStats NewModuleStats, const int NewModuleEquippableFireTypes, TSubclassOf<AOPAG_ModuleBase>& OldModuleClass, EModuleRarity& OldModuleRarity, FOPAG_GunStats& OldModuleStats, int& OldModuleEquippableFireTypes)
{
	if (NewModule)
	{
		// Initialize Base (CDO of NewModule for default params of class) and ModuleSlot (the module of main socket ex. Barrel)
		AOPAG_ModuleBase* Base = NewModule->GetDefaultObject<AOPAG_ModuleBase>();
		UChildActorComponent* ModuleSlot = GetSocketByModuleType(Base->Type);
		AOPAG_ModuleBase* OldModule = GetModuleBySocketType(Base->Type);

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
		AmmoCurrent = Stats.ClipSize;
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