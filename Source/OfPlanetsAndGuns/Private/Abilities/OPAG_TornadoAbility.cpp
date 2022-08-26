// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/OPAG_TornadoAbility.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"

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


}

void AOPAG_TornadoAbility::Init(EAbilityTypes AbilityType)
{
	
}
