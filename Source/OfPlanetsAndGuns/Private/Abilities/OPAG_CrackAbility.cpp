// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/OPAG_CrackAbility.h"
#include "NiagaraComponent.h"
#include "Components/BoxComponent.h"
#include "Characters/Player/OPAG_Player.h"

AOPAG_CrackAbility::AOPAG_CrackAbility()
{
	PrimaryActorTick.bCanEverTick = true;

	AbilityCollider = CreateDefaultSubobject<UBoxComponent>("Collider");
	AbilityCollider->SetupAttachment(Pivot);

	AbilityParticle = CreateDefaultSubobject<UNiagaraComponent>("Particle");
	AbilityParticle->SetupAttachment(AbilityCollider);
	AbilityParticle->bAutoActivate = true;
}

void AOPAG_CrackAbility::BeginPlay()
{
	Super::BeginPlay();

	if (EnableCollisionAfterSeconds > 0)
	{
		AbilityCollider->SetCollisionProfileName(TEXT("NoCollision"));
	}

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AOPAG_CrackAbility::EnableCollision, EnableCollisionAfterSeconds, false);

	AbilityCollider->OnComponentBeginOverlap.AddDynamic(this, &AOPAG_CrackAbility::OnOverlapBegin);
	AbilityCollider->OnComponentEndOverlap.AddDynamic(this, &AOPAG_CrackAbility::OnOverlapEnd);
}

void AOPAG_CrackAbility::Tick(float DeltaTime)
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

void AOPAG_CrackAbility::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AOPAG_Player* Player = Cast<AOPAG_Player>(OtherActor))
	{
		OverlappingTarget = Player;
	}
}

void AOPAG_CrackAbility::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AOPAG_Player* Player = Cast<AOPAG_Player>(OtherActor))
	{
		DamageTimer = 0;
		OverlappingTarget = nullptr;
	}
}

void AOPAG_CrackAbility::EnableCollision()
{
	AbilityCollider->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
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

void AOPAG_CrackAbility::DealDamage(AActor* TargetActor)
{
	DamageTimer = DamageOverTimeFrequency;
	if (AOPAG_Player* Player = Cast<AOPAG_Player>(TargetActor))
	{
		Player->ReceiveDamage(AbilityDamage, this);
	}
}