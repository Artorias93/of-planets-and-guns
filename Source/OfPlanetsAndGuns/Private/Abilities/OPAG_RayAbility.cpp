// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/OPAG_RayAbility.h"
#include "NiagaraComponent.h"
#include "Components/BoxComponent.h"
#include "Characters/OPAG_CharacterBase.h"

AOPAG_RayAbility::AOPAG_RayAbility()
{
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
}

void AOPAG_RayAbility::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AOPAG_CharacterBase* Character = Cast<AOPAG_CharacterBase>(OtherActor))
	{
		Character->ReceiveDamage(AbilityDamage);
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

