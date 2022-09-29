#include "Items/OPAG_Barrel.h"
#include <Kismet/GameplayStatics.h>
#include "Characters/Enemies/OPAG_Enemy.h"
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/KismetMathLibrary.h>
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraShakeBase.h"
#include "Managers/OPAG_SoundManagerSubsystem.h"

AOPAG_Barrel::AOPAG_Barrel()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	if (CapsuleComponent != nullptr)
	{
		RootComponent = CapsuleComponent;
	}

	BarrelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BarrelMesh"));
	if (BarrelMesh != nullptr)
	{
		BarrelMesh->SetupAttachment(RootComponent);
	}
}

void AOPAG_Barrel::BeginPlay()
{
	Super::BeginPlay();
	SMS = GetGameInstance<UOPAG_GameInstance>()->GetSoundManager();

}

void AOPAG_Barrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AOPAG_Barrel::Detonate()
{
	bIsDetonating = true;

	DamageAllTargetsInRange();

	const FVector EmitterSpawnLocation = BarrelMesh->GetComponentLocation();
	const FRotator EmitterSpawnRotation = BarrelMesh->GetComponentRotation();

	// Spawn emitter in scene
	const FVector EmitterSpawnScale = UKismetMathLibrary::Conv_FloatToVector(FMath::RandRange(.8f, 1.2f));
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEmitter, EmitterSpawnLocation, EmitterSpawnRotation, EmitterSpawnScale, true, EPSCPoolMethod::None, true);

	// Spawn decal in scene
	const FVector DecalSpawnSize = UKismetMathLibrary::Conv_FloatToVector(FMath::RandRange(85.f, 125.f));
	UGameplayStatics::SpawnDecalAtLocation(GetWorld(), ExplosionDecal, DecalSpawnSize, EmitterSpawnLocation, FRotator(.0f, -90.f, .0f), .0f);

	// Apply camera shake with custom mintee camera shake
	UGameplayStatics::PlayWorldCameraShake(GetWorld(), ExplosionShake, EmitterSpawnLocation, .0f, 2000.f, 1.f, false);
	
	// Apply sound effect
	SMS->PlayAbiltySounds(this, EAbilitySoundAction::GL_Explosion);

	Destroy();
}

void AOPAG_Barrel::DamageAllTargetsInRange()
{
	TArray<FHitResult> OutHits;
	TArray<AActor*> ActorsToIgnore;
	bool bHasHit = UKismetSystemLibrary::CapsuleTraceMulti(GetWorld(), GetActorLocation(), GetActorForwardVector() + GetActorLocation(),
		Radius, .0f, ETraceTypeQuery::TraceTypeQuery_MAX, true, ActorsToIgnore, EDrawDebugTrace::None, OutHits, true);
	if (bHasHit)
	{
		TArray<AOPAG_CharacterBase*> HitCharacters;
		for (FHitResult OutHit : OutHits)
		{
			AOPAG_CharacterBase* Character = Cast<AOPAG_CharacterBase>(OutHit.GetActor());
			if (Character) HitCharacters.AddUnique(Character);
		}
		for (AOPAG_CharacterBase* Character : HitCharacters)
		{
			Character->ReceiveDamage(Damage, this, false);
		}

		TArray<AOPAG_Barrel*> HitBarrels;
		for (FHitResult OutHit : OutHits)
		{
			AOPAG_Barrel* Other = Cast<AOPAG_Barrel>(OutHit.GetActor());
			if(Other)	HitBarrels.AddUnique(Other);
		}
		for (AOPAG_Barrel* Other : HitBarrels)
		{
			if(Other != nullptr && !Other->IsDetonating())	Other->Detonate();
		}
	}
}