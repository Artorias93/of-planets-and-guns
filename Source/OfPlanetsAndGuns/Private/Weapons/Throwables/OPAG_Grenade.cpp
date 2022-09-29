#include "Weapons/Throwables/OPAG_Grenade.h"

#include "Characters/Enemies/OPAG_Enemy.h"
#include "Managers/OPAG_GameInstance.h"
#include "Managers/OPAG_SoundManagerSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Items/OPAG_Barrel.h"

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
		EDrawDebugTrace::None, OutHits, true);
	if (bHasHit)
	{
		TArray<AOPAG_Enemy*> HitEnemies;
		for (FHitResult OutHit : OutHits)
		{
			AOPAG_Enemy* Enemy = Cast<AOPAG_Enemy>(OutHit.GetActor());
			if (Enemy) HitEnemies.AddUnique(Enemy);

			AOPAG_Barrel* Barrel = Cast<AOPAG_Barrel>(OutHit.GetActor());
			if (Barrel) Barrel->Detonate();
		}
		for (AOPAG_Enemy* Enemy : HitEnemies)
		{
			Enemy->ReceiveDamage(Damage, this, false);
		}
	}
}

void AOPAG_Grenade::BeginPlay()
{
	Super::BeginPlay();
	SMS = GetGameInstance<UOPAG_GameInstance>()->GetSoundManager();
	
}

void AOPAG_Grenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AOPAG_Grenade::Detonate()
{
	DamageAllTargetsInRange();

	// Spawn emitter in scene
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EmitterTemplate, GetActorLocation(), FRotator::ZeroRotator, FVector(10.f, 10.f, 10.f), true, EPSCPoolMethod::None, true);

	// Apply camera shake with custom mintee camera shake
	UGameplayStatics::PlayWorldCameraShake(GetWorld(), ExplosionShake, GetActorLocation(), .0f, 2000.f, 1.f, false);

	// Apply sound effect
	SMS->PlayAbiltySounds( this , EAbilitySoundAction::GL_Explosion);

	Destroy();
}

float AOPAG_Grenade::GetDamage() const
{
	return Damage;
}

FVector AOPAG_Grenade::GetImpactPoint() const
{
	return ImpactPoint;
}

void AOPAG_Grenade::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Detonate();
}
