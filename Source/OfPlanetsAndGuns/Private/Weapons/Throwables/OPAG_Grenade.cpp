#include "Weapons/Throwables/OPAG_Grenade.h"

#include "Characters/Enemies/OPAG_Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

AOPAG_Grenade::AOPAG_Grenade()
{
	PrimaryActorTick.bCanEverTick = true;

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
			Enemy->ReceiveDamage(Damage, this);
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
