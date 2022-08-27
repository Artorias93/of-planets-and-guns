// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/OPAG_CrackAbility.h"
#include "NiagaraComponent.h"
#include "Components/BoxComponent.h"
#include "Characters/OPAG_CharacterBase.h"

AOPAG_CrackAbility::AOPAG_CrackAbility()
{
	AbilityCollider = CreateDefaultSubobject<UBoxComponent>("Collider");
	AbilityCollider->SetupAttachment(Pivot);

	AbilityParticle = CreateDefaultSubobject<UNiagaraComponent>("Particle");
	AbilityParticle->SetupAttachment(AbilityCollider);
	AbilityParticle->bAutoActivate = true;
}

void AOPAG_CrackAbility::BeginPlay()
{
	Super::BeginPlay();

	AbilityCollider->OnComponentBeginOverlap.AddDynamic(this, &AOPAG_CrackAbility::OnOverlapBegin);
}

void AOPAG_CrackAbility::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AOPAG_CharacterBase* Character = Cast<AOPAG_CharacterBase>(OtherActor))
	{
		Character->ReceiveDamage(AbilityDamage);
	}
}

void AOPAG_CrackAbility::DestroyAbility()
{
	AbilityCollider->SetCollisionProfileName(TEXT("NoCollision"));
	this->SetActorEnableCollision(false);

	Super::DestroyAbility();
}

void AOPAG_CrackAbility::Init(EAbilityTypes AbilityType, float Delay)
{
	Super::Init(AbilityType, Delay);
}


