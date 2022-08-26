// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/OPAG_TornadoAbility.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Characters/OPAG_CharacterBase.h"

AOPAG_TornadoAbility::AOPAG_TornadoAbility()
{
	AbilityCollider = CreateDefaultSubobject<UBoxComponent>("Collider");
	AbilityCollider->SetupAttachment(RootComponent);

	AbilityParticle = CreateDefaultSubobject<UParticleSystemComponent>("Particle");
	AbilityParticle->SetupAttachment(AbilityCollider);
	AbilityParticle->bAutoActivate = true;
}

void AOPAG_TornadoAbility::BeginPlay()
{
	Super::BeginPlay();

	AbilityCollider->OnComponentBeginOverlap.AddDynamic(this, &AOPAG_TornadoAbility::OnOverlapBegin);
}

void AOPAG_TornadoAbility::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AOPAG_CharacterBase* Character = Cast<AOPAG_CharacterBase>(OtherActor))
	{
		Character->ReceiveDamage(50);
	}
}

void AOPAG_TornadoAbility::Init(EAbilityTypes AbilityType)
{
	
}

void AOPAG_TornadoAbility::DestroyAbility()
{
	AbilityCollider->SetCollisionProfileName(TEXT("NoCollision"));
	this->SetActorEnableCollision(false);

	Super::DestroyAbility();
}
