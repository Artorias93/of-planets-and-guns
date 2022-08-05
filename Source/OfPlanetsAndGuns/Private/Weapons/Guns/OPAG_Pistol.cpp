#include "Weapons/Guns/OPAG_Pistol.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Characters/OPAG_CharacterBase.h"
#include <Kismet/GameplayStatics.h>
#include <Engine/StaticMeshActor.h>

AOPAG_Pistol::AOPAG_Pistol()
{
	MuzzleComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("MuzzleComponent"));
	MuzzleComponent->SetupAttachment(RootComponent);
}

void AOPAG_Pistol::BeginPlay()
{
	Super::BeginPlay();

}

void AOPAG_Pistol::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

#pragma region Shooting Behavior
void AOPAG_Pistol::ShootLineTrace(FVector StartShootLocation, FVector Direction, const AOPAG_CharacterBase* ShooterCharacter)
{
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
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitPartile, OutHit.Location, FRotator::ZeroRotator, true, EPSCPoolMethod::None, true);

		AOPAG_CharacterBase* Character = Cast<AOPAG_CharacterBase>(OutHit.GetActor());
		if (Character)
		{
			Character->ReceiveDamage(Stats.Damage);
		}
	}

	// Spawn emitter
	// const FVector MuzzleLocation = GetMuzzle()->GetActorLocation();
	UGameplayStatics::SpawnEmitterAttached(MuzzlePartile, GetMuzzle(), NAME_None, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true, EPSCPoolMethod::None, true);
}

USceneComponent* AOPAG_Pistol::GetMuzzle() const
{
	if (MuzzleComponent->GetChildActor())
	{
		AStaticMeshActor* StaticMeshActor = Cast<AStaticMeshActor>(MuzzleComponent->GetChildActor());
		return StaticMeshActor->GetStaticMeshComponent();
	}
	return nullptr;
}
#pragma endregion Shooting Behavior