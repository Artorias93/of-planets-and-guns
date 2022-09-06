#include "Items/OPAG_Barrel.h"
#include <Kismet/GameplayStatics.h>
#include "Characters/Enemies/OPAG_Enemy.h"
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/KismetMathLibrary.h>
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraShakeBase.h"

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
	
}

void AOPAG_Barrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AOPAG_Barrel::Detonate()
{
	DamageAllTargetsInRange();

	const FVector EmitterSpawnLocation = BarrelMesh->GetComponentLocation();
	const FRotator EmitterSpawnRotation = BarrelMesh->GetComponentRotation();
	const FVector EmitterSpawnScale = UKismetMathLibrary::Conv_FloatToVector(FMath::RandRange(.8f, 1.2f));
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEmitter, EmitterSpawnLocation, EmitterSpawnRotation, EmitterSpawnScale, true, EPSCPoolMethod::None, true);

	const FVector DecalSpawnSize = UKismetMathLibrary::Conv_FloatToVector(FMath::RandRange(275.f, 325.f));
	UGameplayStatics::SpawnDecalAtLocation(GetWorld(), ExplosionDecal, DecalSpawnSize, EmitterSpawnLocation, FRotator(.0f, -90.f, .0f), .0f);

	UGameplayStatics::PlayWorldCameraShake(GetWorld(), ExplosionShake, EmitterSpawnLocation, .0f, 2000.f, 1.f, false);
	
	UGameplayStatics::PlaySound2D(GetWorld(), ExplosionSound);

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEmitter, GetActorLocation(), FRotator::ZeroRotator, FVector(10.f, 10.f, 10.f), true, EPSCPoolMethod::None, true);
	UGameplayStatics::PlaySound2D(GetWorld(), ExplosionSound);
	Destroy();
}

void AOPAG_Barrel::DamageAllTargetsInRange()
{
	TArray<FHitResult> OutHits;
	TArray<AActor*> ActorsToIgnore;
	auto PC = GetWorld()->GetFirstPlayerController();
	if (PC) ActorsToIgnore.Add(PC->GetPawn());
	bool bHasHit = UKismetSystemLibrary::CapsuleTraceMultiByProfile(GetWorld(), GetActorLocation(),
		GetActorForwardVector() + GetActorLocation(), Radius, 0.f, TEXT("Pawn"), false, ActorsToIgnore,
		EDrawDebugTrace::Persistent, OutHits, true);
	if (bHasHit)
	{
		TArray<AOPAG_Enemy*> HitEnemies;
		for (FHitResult OutHit : OutHits)
		{
			AOPAG_Enemy* Enemy = Cast<AOPAG_Enemy>(OutHit.Actor);
			if (Enemy) HitEnemies.AddUnique(Enemy);
		}
		for (AOPAG_Enemy* Enemy : HitEnemies)
		{
			Enemy->ReceiveDamage(Damage, this);
		}
	}
}