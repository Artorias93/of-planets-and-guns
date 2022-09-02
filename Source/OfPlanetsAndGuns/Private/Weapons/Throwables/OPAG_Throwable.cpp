#include "Weapons/Throwables/OPAG_Throwable.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AOPAG_Throwable::AOPAG_Throwable()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	if (SphereComponent != nullptr)
	{
		SphereComponent->SetSphereRadius(20.f);
		SphereComponent->SetCollisionProfileName("BlockAll");
		SphereComponent->OnComponentHit.AddDynamic(this, &AOPAG_Throwable::OnHit);
		RootComponent = SphereComponent;
	}

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GrenadeMesh"));
	if (Mesh != nullptr)
	{
		Mesh->SetCollisionProfileName("NoCollision");
		Mesh->SetRelativeScale3D(FVector(.1f, .1f, .1f));
		Mesh->SetupAttachment(RootComponent);
	}

	Projectile = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	if (Projectile != nullptr)
	{
		Projectile->InitialSpeed = 3000.f;
		Projectile->MaxSpeed = 3000.f;
		Projectile->bShouldBounce = true;
		Projectile->Velocity = FVector(3000.f, 0, 0);
	}
}

void AOPAG_Throwable::BeginPlay()
{
	Super::BeginPlay();
	
}

void AOPAG_Throwable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AOPAG_Throwable::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	
}