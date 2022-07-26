#include "Weapons/Guns/OPAG_GunBase.h"
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
		ADSCamera->Mobility = EComponentMobility::Movable;
		ADSCamera->SetupAttachment(RootComponent);
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
	
	EMS = GetGameInstance<UOPAG_GameInstance>()->GetEventManager();
	check(EMS);

	CreateAndAttachAllModules();
	PopulateAmmoTable();
}

void AOPAG_GunBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AOPAG_GunBase::PopulateAmmoTable()
{
	const FString Context;
	if (AmmoReloadingMultipliersTable)
	{
		AmmoReloadingMultipliersTable->GetAllRows(Context, AmmoReloadingMultipliers);
	}
	//checkf(AmmoReloadingMultipliersTable.Num() > 0, TEXT("AmmoReloadingMultipliersTable can't be empty"));
}

#pragma region GET
// Return module by child actor of socket
AOPAG_ModuleBase* AOPAG_GunBase::GetModule(UChildActorComponent* ChildeComponent) const
{
	AOPAG_ModuleBase* Module = Cast<AOPAG_ModuleBase>(ChildeComponent->GetChildActor());
	return Module;
}

// Return module by module type
AOPAG_ModuleBase* AOPAG_GunBase::GetModuleBySocketType(const EModuleTypes ModuleType) const
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

UChildActorComponent* AOPAG_GunBase::GetSocketByModuleType(const EModuleTypes ModuleType) const
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
const FName AOPAG_GunBase::GetSocketNameByModuleType(const EModuleTypes ModuleType) const
{
	FString SocketName;
	UEnum::GetValueAsString(ModuleType, SocketName);
	SocketName.RemoveAt(0, 14, true);
	return FName(*SocketName);
}


UCameraComponent* AOPAG_GunBase::GetSightCamera()
{
	if (Sight->GetChildActor())
	{
		AOPAG_SightBase* Module = Cast<AOPAG_SightBase>(Sight->GetChildActor());
		return Module->GetCamera();
	}
	return nullptr;
}

UCameraComponent* AOPAG_GunBase::GetADSCamera()
{
	return ADSCamera;
}
#pragma endregion GET

#pragma region Mathematic Utility
const float AOPAG_GunBase::CalculateRateInSeconds(const float FireRate)
{
	const float Seconds = 1 / (FireRate / 60);
	return Seconds;
}
#pragma endregion Mathematic Utility

#pragma region Create and Attach Modules
void AOPAG_GunBase::CreateAndAttachAllModules()
{
	CreateModule(Barrel, StandardBarrel);
	CreateModule(Stock, StandardStock);
	CreateModule(Grip, StandardGrip);
	CreateModule(Magazine, StandardMagazine);
}

// Is important when generate the basic gun 
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

// Set child create the object in scene and set the actor's child
// After this is important set the specific params of module (when invoke setchild... the module has the default params, not the values in scene)
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

// When player pick new module in scene, the event call this method 
const bool AOPAG_GunBase::AddNewModule(const TSubclassOf<AOPAG_ModuleBase> NewModule, const EModuleRarity NewModuleRarity, const FGunStats NewModuleStats, const int NewModuleEquippableFireTypes, TSubclassOf<AOPAG_ModuleBase>& OldModuleClass, EModuleRarity& OldModuleRarity, FGunStats& OldModuleStats, int& OldModuleEquippableFireTypes)
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
			return Result;
		}
	}
	return false;
}
#pragma endregion Create and Attach Modules

#pragma region Shooting Behavior
// Is the main method to invoke when event is trigger for shoot
// Call line trace method and set fire rate of gun with timer manager
void AOPAG_GunBase::Shoot(FVector StartShootLocation, FVector Direction)
{
	bCanShoot = false;

	ShootLineTrace(StartShootLocation, Direction);

	const float Seconds = CalculateRateInSeconds(Stats.FireRate);
	GetWorld()->GetTimerManager().SetTimer(ResetShootHandle, this, &AOPAG_GunBase::ResetShootTimer, Seconds, false);
}

void AOPAG_GunBase::ShootLineTrace(FVector StartShootLocation, FVector Direction)
{
	//TODO: Recoil Event
	const FVector NoiseDirection = ApplyNoise(Direction);
	const FVector EffectiveDirection = ApplyWeaponRange(StartShootLocation, NoiseDirection);

	// Initialize out of hit and if check is true (hit object in scene) so the actor is cast to character and control the return value
	FHitResult OutHit;
	TArray<AActor*> ActorsToIgnore = CheckWhatActorsIgnoreLinetrace();
	const bool bIsHit = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), StartShootLocation, EffectiveDirection, ObjectTypesQuery, false,
		ActorsToIgnore, EDrawDebugTrace::Persistent, OutHit, true, FLinearColor::Red, FLinearColor::Green, 5.f);

	// If Actor is inheritance by character base, invoke take damage
	if (bIsHit)
	{
		AOPAG_CharacterBase* Character = Cast<AOPAG_CharacterBase>(OutHit.GetActor());
		if (Character)
		{
			Character->ReceiveDamage(Stats.Damage);
		}
	}
	AmmoCurrent--;
}

void AOPAG_GunBase::ShootThrowable(const FRotator SpawnRotation)
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

void AOPAG_GunBase::SpawnThrowable(const FVector SpawnLocation, const FRotator SpawnRotation)
{
	if (Accessory->GetChildActor())
	{
		const AOPAG_AccessoryBase* Module = Cast<AOPAG_AccessoryBase>(Accessory->GetChildActor());
		const TSubclassOf<AOPAG_Grenade> GrenadeClass = Module->Grenade;
		const FActorSpawnParameters SpawnInfo;
		GetWorld()->SpawnActor(GrenadeClass, &SpawnLocation, &SpawnRotation, SpawnInfo);
		AmmoCurrent--;
	}
}

void AOPAG_GunBase::ResetShootTimer()
{
	bCanShoot = true;
}
#pragma endregion Shooting Behavior

#pragma region Shooting Utility
const float AOPAG_GunBase::CalculateSpread()
{
	const float Spread = (SpreadMax * (ShootConeLenght - Stats.Accuracy)) / 100;
	return Spread;
}

const float AOPAG_GunBase::CalculateSpreadRadius()
{
	const float Spread = CalculateSpread();
	const float SpreadRadius = FMath::Tan(Spread / 2) * ShootConeLenght;
	return SpreadRadius;
}

const FVector2D AOPAG_GunBase::CalculateRandPointInCircle()
{
	const float SpreadRadius = CalculateSpreadRadius();
	const float Angle = FMath::RandRange(0.f, 2 * PI);
	const float DistancefromCenter = SpreadRadius * FMath::Sqrt(FMath::RandRange(0.f, 1.f));
	const FVector2D Point(DistancefromCenter * FMath::Cos(Angle), DistancefromCenter * FMath::Sin(Angle));
	return Point;
}

// Apply noise is method for set the accuracy when player shoot with gun
// Calculate direction of shoot thought the rotation and calculate randomizer point into the cross hair circle
const FVector AOPAG_GunBase::ApplyNoise(const FVector Direction)
{
	const FRotator Rotation = Direction.Rotation();
	const FVector2D Point = CalculateRandPointInCircle();
	const FVector SpreadVector = FVector(0, Point.X * SpreadMultiplier, Point.Y * SpreadMultiplier) + FVector::ForwardVector * ShootConeLenght;
	const FVector NewDirection = Rotation.RotateVector(SpreadVector);
	return NewDirection;
}

// The method applicate the max distance of projectile 
const FVector AOPAG_GunBase::ApplyWeaponRange(const FVector StartLocation, const FVector Direction)
{
	const FVector NewDirection = StartLocation + (Direction * ProjectileRange);
	return NewDirection;
}

// Return where objects ignore the projectile of gun
TArray<AActor*> AOPAG_GunBase::CheckWhatActorsIgnoreLinetrace()
{
	TArray<AActor*> Objects;
	Objects.Add(GetParentActor());
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
#pragma endregion Shooting Utility

#pragma region Ammo System
void AOPAG_GunBase::UpdateAmmo()
{
	const int ClipSize = Stats.ClipSize;
	if (ClipSize < AmmoCurrent)
	{
		AmmoMax += AmmoCurrent - ClipSize;
	}
}

void AOPAG_GunBase::Reload()
{
	bIsReloading = true;
	GetWorld()->GetTimerManager().SetTimer(ReloadHandle, this, &AOPAG_GunBase::WaitReloadingTimer, Stats.ReloadingTime, false);
}

void AOPAG_GunBase::IncrementAmmo()
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
void AOPAG_GunBase::WaitReloadingTimer()
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