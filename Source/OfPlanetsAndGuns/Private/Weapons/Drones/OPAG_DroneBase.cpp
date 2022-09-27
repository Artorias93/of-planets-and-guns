#include "Weapons/Drones/OPAG_DroneBase.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include <Kismet/GameplayStatics.h>
#include "Characters/Enemies/OPAG_Enemy.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/KismetMathLibrary.h"

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
}

// Called when the game starts or when spawned
void AOPAG_DroneBase::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetTimerManager().SetTimer(SpawnFinishedHandle, this, &AOPAG_DroneBase::SpawnFinishedTimer, 1.f, false);
	GetWorld()->GetTimerManager().SetTimer(ShootingHandle, this, &AOPAG_DroneBase::FiendEnemisTimer, .5f, true);
}

// Called every frame
void AOPAG_DroneBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	IdleMovement(DeltaTime);
}

void AOPAG_DroneBase::FiendEnemisTimer()
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
			AOPAG_Enemy* Enemy = Cast<AOPAG_Enemy>(HitResult.Actor);
			if (Enemy != nullptr)
			{
				EnemiesTarget.AddUnique(Enemy);
			}
		}
		if(EnemiesTarget.Num() > 0)	ShootingEnemies(EnemiesTarget);
	}
}

void AOPAG_DroneBase::ShootingEnemies(TArray<AOPAG_Enemy*> EnemiesTarget)
{
	int SelectRandomicEnemyIndex = 0;
	const FVector StartLocation = GetActorLocation(); //+ (GetActorForwardVector() * 80.f);
	
	for (int i = 0; i < 2; i++)
	{
		// Select new target enemy on list 
		SelectRandomicEnemyIndex = FMath::RandRange(0, EnemiesTarget.Num() - 1);
		AOPAG_Enemy* TargetEnemy = EnemiesTarget[SelectRandomicEnemyIndex];

		// Calculate effective direction 
		const FVector TargetEnemyLocation = TargetEnemy->GetActorLocation();
		const FVector EffectiveDirection = CalculateEffectiveDirection(StartLocation, TargetEnemyLocation);
		
		// Calculate projectile's location and rotation
		const FVector TraceStartLocation = StartLocation;
		const FRotator EffectiveRotation = EffectiveDirection.Rotation();

		// Start shoot and emitter
		Shoot(TargetEnemy, TraceStartLocation, EffectiveRotation);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzlePartile, GetActorTransform());
	}
}

void AOPAG_DroneBase::Shoot(AOPAG_Enemy* Enemy, const FVector TraceStartLocation, const FRotator EffectiveRotation)
{
	const FTransform TracerTransform = FTransform(EffectiveRotation, TraceStartLocation, FVector(.3f, .3f, .3f));
	GetWorld()->SpawnActor(ProjectileTrace, &TracerTransform);
	
	float EffectiveDamage = CalculateDamage();
	Enemy->ReceiveDamage(EffectiveDamage, GetParentActor());
}

float AOPAG_DroneBase::CalculateDamage() const
{
	const float DamageRandomnessAmount = Damage * 0.01f * DamageRandomnessPercentage;
	float AdjustedDamage = (Damage + FMath::RandRange(-DamageRandomnessAmount, DamageRandomnessAmount)) * MultiplierByLevel;
	return AdjustedDamage;
}

void AOPAG_DroneBase::SpawnFinishedTimer()
{
	if (SpawnParticleSystemComponent != nullptr) 
	{
		SpawnParticleSystemComponent->Deactivate();
		SpawnParticleSystemComponent->DestroyComponent();
	}
}

void AOPAG_DroneBase::IdleMovement(float DeltaTime)
{
	FRotator NewRotation = !bIsReverse ? UKismetMathLibrary::RInterpTo_Constant(BoxCollision->GetRelativeRotation(), 
		FRotator(.0f, .0f, -26.f), DeltaTime, InterpMovementSpeed) : UKismetMathLibrary::RInterpTo_Constant(BoxCollision->GetRelativeRotation(),
		FRotator(.0f, .0f, 26.f), DeltaTime, InterpMovementSpeed);

	BoxCollision->SetRelativeRotation(NewRotation);

	if(BoxCollision->GetRelativeRotation().Roll < -25.f)	bIsReverse = true;
	if(BoxCollision->GetRelativeRotation().Roll > 25.f) bIsReverse = false;
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

void AOPAG_DroneBase::DestroyDrone()
{
	if(ExplosionParticle == nullptr)	return;
	
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticle, GetActorTransform());
	DroneMesh->SetHiddenInGame(true);
}