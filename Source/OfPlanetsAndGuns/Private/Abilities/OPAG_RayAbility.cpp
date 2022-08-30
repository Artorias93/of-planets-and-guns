// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/OPAG_RayAbility.h"
#include "NiagaraComponent.h"
#include "Components/BoxComponent.h"
#include "Characters/Player/OPAG_Player.h"

AOPAG_RayAbility::AOPAG_RayAbility()
{
	PrimaryActorTick.bCanEverTick = true;

	AbilityCollider = CreateDefaultSubobject<UBoxComponent>("Collider");
	AbilityCollider->SetupAttachment(Pivot);

	AbilityParticleMuzzle = CreateDefaultSubobject<UNiagaraComponent>("Particle Muzzle");
	AbilityParticleMuzzle->SetupAttachment(AbilityCollider);
	AbilityParticleMuzzle->bAutoActivate = true;

	AbilityParticleRay = CreateDefaultSubobject<UNiagaraComponent>("Particle Ray");
	AbilityParticleRay->SetupAttachment(AbilityParticleMuzzle);
	AbilityParticleRay->bAutoActivate = true;

	AbilityParticleHit = CreateDefaultSubobject<UNiagaraComponent>("Particle Hit");
	AbilityParticleHit->SetupAttachment(AbilityParticleRay);
	AbilityParticleHit->bAutoActivate = true;
}

void AOPAG_RayAbility::BeginPlay()
{
	Super::BeginPlay();

	AbilityCollider->OnComponentBeginOverlap.AddDynamic(this, &AOPAG_RayAbility::OnOverlapBegin);
	AbilityCollider->OnComponentEndOverlap.AddDynamic(this, &AOPAG_RayAbility::OnOverlapEnd);
}

void AOPAG_RayAbility::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (OverlappingTarget)
	{
		if (DamageTimer <= 0)
			DealDamage(OverlappingTarget);
		else
			DamageTimer -= DeltaTime;
	}
}

void AOPAG_RayAbility::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AOPAG_Player* Player = Cast<AOPAG_Player>(OtherActor))
	{
		OverlappingTarget = Player;
	}
}

void AOPAG_RayAbility::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AOPAG_Player* Player = Cast<AOPAG_Player>(OtherActor))
	{
		DamageTimer = 0;
		OverlappingTarget = nullptr;
	}
}

void AOPAG_RayAbility::Init(EAbilityTypes AbilityType, float Delay)
{
	Super::Init(AbilityType, Delay);
}

void AOPAG_RayAbility::DestroyAbility()
{
	AbilityCollider->SetCollisionProfileName(TEXT("NoCollision"));
	this->SetActorEnableCollision(false);

	Super::DestroyAbility();
}

void AOPAG_RayAbility::DealDamage(AActor* TargetActor)
{
	DamageTimer = DamageOverTimeFrequency;
	if (AOPAG_Player* Player = Cast<AOPAG_Player>(TargetActor))
	{
		Player->ReceiveDamage(AbilityDamage, this);
	}
}
