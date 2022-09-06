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
#include "Characters/Enemies/OPAG_Enemy.h"
#include <Kismet/GameplayStatics.h>
#include "Abilities/OPAG_ForceField.h"
#include "Characters/Enemies/ShieldEnemy/OPAG_ShieldEnemy.h"
#include "Weapons/Modules/OPAG_ModuleStats.h"
#include "Items/OPAG_Barrel.h"

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
}

void AOPAG_GunBase::BeginPlay()
{
	Super::BeginPlay();

	EMS = GetGameInstance<UOPAG_GameInstance>()->GetEventManager();
	check(EMS);

	AmmoCurrent = GetStatsTotal().ClipSize;
}

void AOPAG_GunBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

#pragma region Get
UCameraComponent* AOPAG_GunBase::GetADSCamera() const
{
	return ADSCamera;
}

void AOPAG_GunBase::ResetReductionSpreadRadiusShooting()
{
	ReductionSpreadRadiusShooting = 1.f;
}

FName AOPAG_GunBase::GetGunName() const
{
	return Name;
}

EFireTypes AOPAG_GunBase::GetFireType() const
{
	return FireType;
}

FOPAG_GunStats AOPAG_GunBase::GetStatsTotal() const
{
	return StatsBase;
}

int AOPAG_GunBase::GetAmmoCurrent() const
{
	return AmmoCurrent;
}

int AOPAG_GunBase::GetAmmoMax() const
{
	return AmmoMax;
}

float AOPAG_GunBase::GetRecoilStrength() const
{
	return RecoilStrength;
}

float AOPAG_GunBase::GetRecoilSpeed() const
{
	return RecoilSpeed;
}
#pragma endregion Get

#pragma region Set
void AOPAG_GunBase::SetStatsTotal(const FOPAG_GunStats NewStats)
{
	StatsBase = NewStats;
}

void AOPAG_GunBase::SetAmmoCurrent(const int Value)
{
	AmmoCurrent = Value;
}

void AOPAG_GunBase::SetAmmoMax(const int Value)
{
	AmmoMax = Value;
}

void AOPAG_GunBase::DecrementAmmoCurrent(const int Value)
{
	AmmoCurrent -= Value;
}
#pragma endregion Set

#pragma region Mathematic Utility
float AOPAG_GunBase::CalculatePeriod(const float FireRate)
{
	return SecondsInOneMinute / FireRate;
}
#pragma endregion Mathematic Utility

#pragma region Shooting Behavior
// Main method to to call when a shooting event is triggered
// Call line trace method and set fire rate of gun with timer manager
int AOPAG_GunBase::Shoot(const FVector StartShootLocation, const FVector Direction, const AOPAG_CharacterBase* ShooterCharacter)
{
	ShootLineTrace(StartShootLocation, Direction, ShooterCharacter);

	ReductionSpreadRadiusShooting = FMath::Clamp(ReductionSpreadRadiusShooting + .15f, 1.f, ReductionSpreadRadiusShootingMax);

	UGameplayStatics::PlaySound2D(GetWorld(), ShootSound);
	if(AOPAG_CharacterBase* const CharacterOwner = Cast<AOPAG_CharacterBase>(GetParentActor()))
	{
		MakeNoise(1.f, CharacterOwner, CharacterOwner->GetActorLocation(), 0);
	}	

	return 1;
}

void AOPAG_GunBase::ShootLineTrace(const FVector StartShootLocation, const FVector Direction, const AOPAG_CharacterBase* ShooterCharacter)
{
	//TODO: Recoil Event
	const FVector NoiseDirection = ApplyNoise(Direction);
	const FVector EffectiveDirection = ApplyWeaponRange(StartShootLocation, NoiseDirection);

	// Initialize out of hit and if check is true (hit object in scene) so the actor is cast to character and control the return value
	FHitResult OutHit;
	const TArray<AActor*> ActorsToIgnore = CheckWhatActorsIgnoreLinetrace();	
	const bool bIsHit = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), StartShootLocation, EffectiveDirection, ObjectTypesQuery, false,
		ActorsToIgnore, EDrawDebugTrace::None, OutHit, true, FLinearColor::Red, FLinearColor::Green, 5.f);

	// If Actor is inherited by character base, invoke take damage
	if (bIsHit)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitPartile, OutHit.Location, FRotator::ZeroRotator, true, EPSCPoolMethod::None, true);
		/*
		FHitResult OutHit2;
		const bool bIsHit2 = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), MuzzleLocation, OutHit.Location, ObjectTypesQuery, false,
			ActorsToIgnore, EDrawDebugTrace::ForDuration, OutHit2, true, FLinearColor::Red, FLinearColor::Green, 5.f);
		
		const FVector LookDirection = bIsHit2 ? OutHit2.ImpactPoint : OutHit2.TraceEnd;
		const FRotator LookRotator = UKismetMathLibrary::FindLookAtRotation(MuzzleLocation, LookDirection);
		const FTransform TracerTransform(MuzzleLocation, LookRotator, FVector(.3f, .3f, .3f));

		// Spawn trace in the space
	    GetWorld()->SpawnActor(TracerClass, TracerTransform);*/


		// It should be implemented by a "Hittable interface, were the gun just check if the hitted actor has this interface, if yes, send the an hit event with the hitted actor as param"
		if (AOPAG_ForceField* ForceField = Cast<AOPAG_ForceField>(OutHit.GetActor()))
		{
			EMS->ForceFieldHit.Broadcast(ForceField);
		}


		AOPAG_CharacterBase* Character = Cast<AOPAG_CharacterBase>(OutHit.GetActor());
		if (Character)
		{
			//Checks for friendly fire
			//If both the shooter and the target are enemies, damage is not applied
			const AOPAG_Enemy* const EnemyShooter = Cast<AOPAG_Enemy>(ShooterCharacter);
			const AOPAG_Enemy* const EnemyShot = Cast<AOPAG_Enemy>(Character);
			if(!(EnemyShooter && EnemyShot))
			{
				//Check for shield enemy
				//If the hit component is the shield, damage is not applied
				if(!OutHit.GetComponent()->ComponentHasTag("Shield"))
				{
					//Check for headshot
					const bool IsHeadshot = OutHit.GetComponent()->ComponentHasTag("Head");
					Character->ReceiveDamage(CalculateDamage(IsHeadshot), GetParentActor());
				}
			}
		}
		AOPAG_Barrel* Barrel = Cast<AOPAG_Barrel>(OutHit.GetActor());
		if (Barrel != nullptr)
		{
			Barrel->Detonate();
		}
	}
	
	// Spawn emitter
	// const FVector MuzzleLocation = GetMuzzle()->GetActorLocation();
	UGameplayStatics::SpawnEmitterAttached(MuzzlePartile, GetMuzzle(), NAME_None, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true, EPSCPoolMethod::None, true);
}

USceneComponent* AOPAG_GunBase::GetMuzzle() const
{
	return nullptr;
}

void AOPAG_GunBase::ResetShootTimer()
{
	bCanShoot = true;
}
#pragma endregion Shooting Behavior

#pragma region Shooting Utility
float AOPAG_GunBase::CalculateDamage(const bool IsHeadshot) const
{
	const float DamageRandomnessAmount = GetStatsTotal().Damage * 0.01f * DamageRandomnessPercentage;
	float AdjustedDamage = GetStatsTotal().Damage + FMath::RandRange(-DamageRandomnessAmount, DamageRandomnessAmount);
	if (IsHeadshot || FMath::RandRange(0.f, 100.f) < BaseCritChance)
	{
		AddCritToDamage(AdjustedDamage);
	}
	return AdjustedDamage;
}

void AOPAG_GunBase::AddCritToDamage(float& Damage) const
{
	Damage *= CritDamageMultiplier;
}

float AOPAG_GunBase::CalculateSpread() const
{
	// Only use for aiming behavior
	AOPAG_Player* Player = Cast<AOPAG_Player>(GetParentActor());
	if(Player == nullptr)	return .0f;

	const float ADSAccuracy = (100 + GetStatsTotal().Accuracy) / 2;
	static constexpr float InternalRange = MaxInternalAccuracy - MinInternalAccuracy;
	const float InternalAccuracy = ((
		(Player->GetIsADS() ? ADSAccuracy : GetStatsTotal().Accuracy) 
		* InternalRange) / 100.f) + MinInternalAccuracy;
	const float ClampedAccuracy = FMath::Clamp(InternalAccuracy, MinInternalAccuracy, MaxInternalAccuracy);
	const float Spread = (SpreadMax * (ShootConeLength - ClampedAccuracy)) / 100;
	return Spread;
}

float AOPAG_GunBase::CalculateSpreadRadius() const
{
	const float Spread = CalculateSpread();
	const float SpreadRadius = FMath::Tan(Spread / 2) * ShootConeLength;
	
	const AOPAG_CharacterBase* Character = Cast<AOPAG_CharacterBase>(GetParentActor());
	if (Character)
	{
		const float Velocity = Character->GetVelocity().Size();
		if (Velocity > .1f)
		{
			return SpreadRadius * ReductionSpreadRadius;
		}
	}
	return SpreadRadius * ReductionSpreadRadiusShooting;
}

FVector2D AOPAG_GunBase::CalculateRandPointInCircle() const
{
	const float SpreadRadius = CalculateSpreadRadius();
	const float Angle = FMath::RandRange(0.f, 2 * PI);
	const float DistanceFromCenter = SpreadRadius * FMath::Sqrt(FMath::RandRange(0.f, 1.f));
	const FVector2D Point(DistanceFromCenter * FMath::Cos(Angle), DistanceFromCenter * FMath::Sin(Angle));
	return Point;
}

// Apply noise is method for set the accuracy when player shoot with gun
// Calculate direction of shoot thought the rotation and calculate randomizer point into the cross hair circle
FVector AOPAG_GunBase::ApplyNoise(const FVector Direction) const
{
	const FRotator Rotation = Direction.Rotation();
	const FVector2D Point = CalculateRandPointInCircle();
	const FVector SpreadVector = FVector(0, Point.X * SpreadMultiplier, Point.Y * SpreadMultiplier) + FVector::ForwardVector * ShootConeLength;
	const FVector NewDirection = Rotation.RotateVector(SpreadVector);
	return NewDirection;
}

// The method applies the max distance of projectile 
FVector AOPAG_GunBase::ApplyWeaponRange(const FVector StartLocation, const FVector Direction) const
{
	const FVector NewDirection = StartLocation + (Direction * ProjectileRange);
	return NewDirection;
}

// Return where objects ignore the projectile of gun
TArray<AActor*> AOPAG_GunBase::CheckWhatActorsIgnoreLinetrace()
{
	TArray<AActor*> Objects;
	this->GetAllChildActors(Objects);
	Objects.Add(GetParentActor());
	Objects.Add(this);
	
	if (const AOPAG_Player* const Player = Cast<AOPAG_Player>(GetParentActor()))
	{
		if (Player->GetShield() != nullptr)
		{
			Objects.Add(Player->GetShield());
		}
	}

	return Objects;
}
#pragma endregion Shooting Utility

#pragma region Reload
void AOPAG_GunBase::Reload()
{
	bIsReloading = true;
	GetWorld()->GetTimerManager().SetTimer(ReloadHandle, this, &AOPAG_GunBase::WaitReloadingTimer, GetStatsTotal().ReloadingTime, false);
	UGameplayStatics::PlaySound2D(GetWorld(), ReloadSound);
}

void AOPAG_GunBase::WaitReloadingTimer()
{
	// Check the bool when interrupter the reloading
	bIsReloading = false;

	AmmoCurrent = GetStatsTotal().ClipSize;
	EMS->CurrentAmmoChanged.Broadcast(AmmoCurrent);
}
#pragma endregion Reload

#pragma region Checking
bool AOPAG_GunBase::CanShoot() const
{
	return bCanShoot && !bIsReloading && AmmoCurrent > 0;
}

bool AOPAG_GunBase::CanReloading() const
{
	return !bIsReloading && AmmoCurrent < GetStatsTotal().ClipSize && AmmoMax > 0;
}

bool AOPAG_GunBase::CanAim() const
{
	return !bIsReloading;
}
#pragma endregion Checking
