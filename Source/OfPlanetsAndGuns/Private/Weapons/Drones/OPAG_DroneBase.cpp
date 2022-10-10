#include "Weapons/Drones/OPAG_DroneBase.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include <Kismet/GameplayStatics.h>
#include "Characters/Enemies/OPAG_Enemy.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Characters/Player/OPAG_Player.h"
#include <Components/TimelineComponent.h>
#include "Managers/OPAG_GameInstance.h"
#include "Managers/OPAG_SoundManagerSubsystem.h"

AOPAG_DroneBase::AOPAG_DroneBase()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("SphereCollision"));
	if (BoxCollision != nullptr)
	{
		BoxCollision->SetBoxExtent(FVector(600.f, 50.f, 20.f));
		BoxCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
		RootComponent = BoxCollision;
	}

	DroneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DroneMesh"));
	if (DroneMesh != nullptr)
	{
		DroneMesh->SetRelativeRotation(FRotator(.0f, 270.f, .0f));
		DroneMesh->SetRelativeScale3D(FVector(.04f, .04f, .04f));
		DroneMesh->SetCollisionProfileName(TEXT("NoCollision"));
		DroneMesh->SetupAttachment(RootComponent);
	}

	FirstShootPoint = CreateDefaultSubobject<USceneComponent>(TEXT("FirstShootPoint"));
	if (FirstShootPoint != nullptr)
	{
		FirstShootPoint->SetRelativeLocation(FVector(-30.f, 10.f, 0.f));
		FirstShootPoint->SetupAttachment(RootComponent);
	}

	SecondShootPoint = CreateDefaultSubobject<USceneComponent>(TEXT("SecondShootPoint"));
	if (SecondShootPoint != nullptr)
	{
		SecondShootPoint->SetRelativeLocation(FVector(30.f, 10.f, 0.f));
		SecondShootPoint->SetupAttachment(RootComponent);
	}

	BurstParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("BurstParticleSystem"));
	if (BurstParticleSystemComponent != nullptr)
	{
		BurstParticleSystemComponent->SetupAttachment(RootComponent);
	}

	SpawnParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SpawnParticleSystemComponent"));
	if (SpawnParticleSystemComponent != nullptr)
	{
		SpawnParticleSystemComponent->SetupAttachment(RootComponent);
	}

	SpawnTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("SpawnTimeline"));
}

// Called when the game starts or when spawned
void AOPAG_DroneBase::BeginPlay()
{
	Super::BeginPlay();

	SMS = GetGameInstance<UOPAG_GameInstance>()->GetSoundManager();
	
	// Spawn timeline
	InitTimeline();
	
	// Init timer for find enemies
	GetWorld()->GetTimerManager().SetTimer(ShootingHandle, this, &AOPAG_DroneBase::FindEnemisTimer, .5f, true);
}

// Called every frame
void AOPAG_DroneBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckPlayerDistance();
	FollowPlayer(DeltaTime);
}

void AOPAG_DroneBase::InitTimeline()
{
	SpawnTimeline->SetLooping(false);

	OnSpawnTimelineProgress.BindUFunction(this, FName("SpawnTimelineProgress"));
	SpawnTimeline->AddInterpVector(SpawnCurveVector, OnSpawnTimelineProgress, FName("SpawnTimelinePropertyName"));

	OnSpawnTimelineFinished.BindUFunction(this, FName("SpawnTimelineFinished"));
	SpawnTimeline->SetTimelineFinishedFunc(OnSpawnTimelineFinished);
}

void AOPAG_DroneBase::DestroyDrone()
{
	if(ExplosionParticle == nullptr)	return;
	
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticle, GetActorTransform());
	DroneMesh->SetHiddenInGame(true);
}

#pragma region Shoot
void AOPAG_DroneBase::FindEnemisTimer()
{
	TArray<FHitResult> OutHits;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetParentActor());
	bool bResult = UKismetSystemLibrary::SphereTraceMulti(GetWorld(), GetActorLocation(), GetActorLocation(), VisibilityRange,
		ETraceTypeQuery::TraceTypeQuery_MAX, true, ActorsToIgnore, EDrawDebugTrace::None, OutHits, true);
	if (bResult)
	{
		TArray<AOPAG_Enemy*> EnemiesTarget;
		for (FHitResult HitResult : OutHits)
		{
			AOPAG_Enemy* Enemy = Cast<AOPAG_Enemy>(HitResult.GetActor());
			if (Enemy != nullptr)
			{
				EnemiesTarget.AddUnique(Enemy);
			}
		}
		if (EnemiesTarget.Num() > 0)	ShootingEnemies(EnemiesTarget);
		else  bIsShooting = false;
	}
}

void AOPAG_DroneBase::ShootingEnemies(TArray<AOPAG_Enemy*> EnemiesTarget)
{
	// Check shooting
	bIsShooting = true;

	// Select randomize enemy
	int SelectRandomicEnemyIndex = FMath::RandRange(0, EnemiesTarget.Num() - 1);
	TargetEnemy = EnemiesTarget[SelectRandomicEnemyIndex];

	// Start Location
	const FVector StartLocation = GetActorLocation() + (GetActorForwardVector() * 80.f);

	// Calculate effective direction 
	const FVector TargetEnemyLocation = TargetEnemy->GetActorLocation();
	const FVector EffectiveDirection = CalculateEffectiveDirection(StartLocation, TargetEnemyLocation);

	// Calculate projectile's location and rotation
	const FVector TraceStartLocation = StartLocation;
	const FRotator EffectiveRotation = EffectiveDirection.Rotation();

	for (int i = 0; i < 2; i++)
	{
		Shoot(TargetEnemy, TraceStartLocation, EffectiveRotation);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzlePartile, GetActorTransform());
	}
}

void AOPAG_DroneBase::Shoot(AOPAG_Enemy* Enemy, const FVector TraceStartLocation, const FRotator EffectiveRotation)
{
	const FTransform TracerTransform = FTransform(EffectiveRotation, TraceStartLocation, FVector(.3f, .3f, .3f));
	GetWorld()->SpawnActor(ProjectileTrace, &TracerTransform);

	float EffectiveDamage = CalculateDamage();
	Enemy->ReceiveDamage(EffectiveDamage, GetParentActor(), false); // temporarily crit set to false

	// Implement sound ---
}

float AOPAG_DroneBase::CalculateDamage() const
{
	const float DamageRandomnessAmount = Damage * 0.01f * DamageRandomnessPercentage;
	float AdjustedDamage = (Damage + FMath::RandRange(-DamageRandomnessAmount, DamageRandomnessAmount)) * MultiplierByLevel;
	return AdjustedDamage;
}

const FVector AOPAG_DroneBase::CalculateEffectiveDirection(const FVector StartLocation, const FVector EnemyLocation) const
{
	const FVector EnemyToSelfVector = EnemyLocation - StartLocation;
	float ENemyToSelfLength = EnemyLocation.Size();

	FVector TangentVector1 = FVector::CrossProduct(EnemyToSelfVector, FVector::UpVector);
	TangentVector1.Normalize();
	FVector TangentVector2 = FVector::CrossProduct(EnemyToSelfVector, TangentVector1);
	TangentVector2.Normalize();

	float RandomAngle1 = FMath::RandRange(-RandomShootingAngle, RandomShootingAngle);
	float RandomAngle2 = FMath::RandRange(-RandomShootingAngle, RandomShootingAngle);

	float RandomFactor1 = FMath::Tan((RandomAngle1 * PI) / 180) * ENemyToSelfLength;
	TangentVector1 *= RandomFactor1;
	float RandomFactor2 = FMath::Tan((RandomAngle2 * PI) / 180) * ENemyToSelfLength;
	TangentVector2 *= RandomFactor2;

	const FVector EffectiveDirection = EnemyToSelfVector + TangentVector1 + TangentVector2;
	return EffectiveDirection;
}
#pragma endregion Shoot

#pragma region Movement
void AOPAG_DroneBase::FollowPlayer(float DeltaTime)
{
	AOPAG_Player* Player = Cast<AOPAG_Player>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (Player == nullptr || !bIsFollowing)	return;

	const FVector PlayerLocation = Player->GetActorLocation();
	const FVector OffesetLocation = PlayerLocation + (Player->GetActorUpVector() * 80.f) + (-Player->GetActorForwardVector() * 80.f);
	const FVector InterpMovement = FMath::VInterpConstantTo(GetActorLocation(), OffesetLocation, DeltaTime, MovementSpeed);
	BoxCollision->SetWorldLocation(InterpMovement);

	const FRotator TargetRotation = !bIsShooting ? (-Player->GetActorForwardVector()).Rotation() :
		(-TargetEnemy->GetActorForwardVector()).Rotation();
	const FRotator IterpRotation = FMath::RInterpConstantTo(GetActorRotation(), TargetRotation, DeltaTime, RotationSpeed);
	BoxCollision->SetWorldRotation(IterpRotation);

	// New Check Distance
	float PlayerDistanceToSelf = FVector::Distance(OffesetLocation, GetActorLocation());
	if (PlayerDistanceToSelf < 20.f)
	{
		BoxCollision->SetWorldRotation(Player->GetActorForwardVector().Rotation());
		bIsFollowing = false;
	}
}

void AOPAG_DroneBase::CheckPlayerDistance()
{
	AOPAG_Player* Player = Cast<AOPAG_Player>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (Player != nullptr && !bIsFollowing)
	{
		float PlayerDistanceToSelf = FVector::Distance(Player->GetActorLocation(), GetActorLocation());
		if (PlayerDistanceToSelf > PlayerDistanceToSelfMax)
		{
			bIsFollowing = true;
		}
	}
}
#pragma endregion Movement

#pragma region Spawn methods
void AOPAG_DroneBase::StartSpawnTimeline()
{
	SpawnLocation = GetActorLocation();
	BoxCollision->SetSimulatePhysics(true);

	// Find a random angle in order to determine the direction where the module will drop
	float RandomAngle = FMath::DegreesToRadians(FMath::RandRange(0, 360));
	RandomSpawnDirection.X = FMath::Sin(RandomAngle);
	RandomSpawnDirection.Y = FMath::Cos(RandomAngle);

	SpawnTimeline->PlayFromStart();
}

void AOPAG_DroneBase::SpawnTimelineProgress(FVector Value)
{
	FVector NewLocation = SpawnLocation;
	NewLocation.X += RandomSpawnDirection.X * Value.X * SpawCurveXFactor;
	NewLocation.Y += RandomSpawnDirection.Y * Value.X * SpawCurveXFactor;
	NewLocation.Z += Value.Z * SpawCurveZFactor;

	SetActorLocation(NewLocation);
}

void AOPAG_DroneBase::SpawnTimelineFinished()
{
	SpawnParticleSystemComponent->Deactivate();
	SpawnParticleSystemComponent->DestroyComponent();

	BoxCollision->SetSimulatePhysics(false);
}
#pragma endregion Spawn methods

#pragma region Temporaly
void AOPAG_DroneBase::IdleMovement(float DeltaTime)
{
	FRotator NewRotation = !bIsReverse ? UKismetMathLibrary::RInterpTo_Constant(BoxCollision->GetRelativeRotation(),
		FRotator(.0f, .0f, -26.f), DeltaTime, InterpMovementSpeed) : UKismetMathLibrary::RInterpTo_Constant(BoxCollision->GetRelativeRotation(),
			FRotator(.0f, .0f, 26.f), DeltaTime, InterpMovementSpeed);

	BoxCollision->SetRelativeRotation(NewRotation);

	if (BoxCollision->GetRelativeRotation().Roll < -25.f)	bIsReverse = true;
	if (BoxCollision->GetRelativeRotation().Roll > 25.f) bIsReverse = false;
}
#pragma endregion Temporaly