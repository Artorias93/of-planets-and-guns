#include "Weapons/Throwables/OPAG_Grenade.h"

#include "Characters/Enemies/OPAG_Enemy.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

AOPAG_Grenade::AOPAG_Grenade()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	SphereComponent->SetSphereRadius(20.f);
	SphereComponent->SetCollisionProfileName("BlockAll");
	SphereComponent->OnComponentHit.AddDynamic(this, &AOPAG_Grenade::OnHit);
	RootComponent = SphereComponent;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GrenadeMesh"));
	Mesh->SetCollisionProfileName("NoCollision");
	Mesh->SetRelativeScale3D(FVector(.1f, .1f, .1f));
	Mesh->SetupAttachment(RootComponent);

	Projectile = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	Projectile->InitialSpeed = 3000.f;
	Projectile->MaxSpeed = 3000.f;
	Projectile->bShouldBounce = true;
	Projectile->Velocity = FVector(3000.f, 0, 0);
}

void AOPAG_Grenade::DamageAllTargetsInRange()
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
			Enemy->ReceiveDamage(Damage);
		}
	}
}

void AOPAG_Grenade::BeginPlay()
{
	Super::BeginPlay();
	
}

void AOPAG_Grenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AOPAG_Grenade::Detonate()
{
	DamageAllTargetsInRange();

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EmitterTemplate, GetActorLocation(), FRotator::ZeroRotator, FVector(10.f, 10.f, 10.f), true, EPSCPoolMethod::None, true);
	UGameplayStatics::PlaySound2D(GetWorld(), ExplosionSound);

	Destroy();
}

void AOPAG_Grenade::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Detonate();
}
