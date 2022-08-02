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
}

void AOPAG_GunBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

#pragma region GET
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

#pragma region Shooting Behavior
// Main method to to call when a shooting event is triggered
// Call line trace method and set fire rate of gun with timer manager
void AOPAG_GunBase::Shoot(FVector StartShootLocation, FVector Direction, const AOPAG_CharacterBase* ShooterCharacter)
{
	bCanShoot = false;

	ShootLineTrace(StartShootLocation, Direction, ShooterCharacter);

	const float Seconds = CalculateRateInSeconds(Stats.FireRate);
	GetWorld()->GetTimerManager().SetTimer(ResetShootHandle, this, &AOPAG_GunBase::ResetShootTimer, Seconds, false);
}

void AOPAG_GunBase::ShootLineTrace(FVector StartShootLocation, FVector Direction, const AOPAG_CharacterBase* ShooterCharacter)
{
	//TODO: Recoil Event
	const FVector NoiseDirection = ApplyNoise(Direction);
	const FVector EffectiveDirection = ApplyWeaponRange(StartShootLocation, NoiseDirection);

	// Initialize out of hit and if check is true (hit object in scene) so the actor is cast to character and control the return value
	FHitResult OutHit;
	const TArray<AActor*> ActorsToIgnore = CheckWhatActorsIgnoreLinetrace();	
	const bool bIsHit = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), StartShootLocation, EffectiveDirection, ObjectTypesQuery, false,
		ActorsToIgnore, EDrawDebugTrace::Persistent, OutHit, true, FLinearColor::Red, FLinearColor::Green, 5.f);

	// If Actor is inherited by character base, invoke take damage
	if (bIsHit)
	{
		AOPAG_CharacterBase* Character = Cast<AOPAG_CharacterBase>(OutHit.GetActor());
		if (Character)
		{
			//Checks for friendly fire
			//If both the shooter and the target are enemies, damage is not applied
			const AOPAG_Enemy* const EnemyShooter = Cast<AOPAG_Enemy>(ShooterCharacter);
			const AOPAG_Enemy* const EnemyShot = Cast<AOPAG_Enemy>(Character);
			if(!(EnemyShooter && EnemyShot))
				Character->ReceiveDamage(Stats.Damage);
		}
	}
	AmmoCurrent--;
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
	Objects.Add(this);
	return Objects;
}
#pragma endregion Shooting Utility