// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/OPAG_TornadoAbility.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Characters/Player/OPAG_Player.h"
#include "Math/UnrealMathUtility.h"

AOPAG_TornadoAbility::AOPAG_TornadoAbility()
{
	PrimaryActorTick.bCanEverTick = true;

	AbilityCollider = CreateDefaultSubobject<UBoxComponent>("Collider");
	AbilityCollider->SetupAttachment(Pivot);

	AbilityParticle = CreateDefaultSubobject<UParticleSystemComponent>("Particle");
	AbilityParticle->SetupAttachment(AbilityCollider);
	AbilityParticle->bAutoActivate = true;
}

void AOPAG_TornadoAbility::BeginPlay()
{
	Super::BeginPlay();

	AbilityCollider->OnComponentBeginOverlap.AddDynamic(this, &AOPAG_TornadoAbility::OnOverlapBegin);
	AbilityCollider->OnComponentEndOverlap.AddDynamic(this, &AOPAG_TornadoAbility::OnOverlapEnd);
}

void AOPAG_TornadoAbility::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Target != nullptr)
	{
		MovementDirection = Target->GetActorLocation() - this->GetActorLocation();
		MovementDirection.Normalize();
	}

	FVector Velocity = MovementDirection * Speed * DeltaTime;
	FVector NewLocation = GetActorLocation() + Velocity;

	// keep always the same Z, in order to avoid that the tornado start flying to reach the center of the Target
	NewLocation.Z = GetActorLocation().Z;

	SetActorLocation(NewLocation);

	if (OverlappingTarget)
	{
		if (DamageTimer <= 0)
			DealDamage(OverlappingTarget);
		else
			DamageTimer -= DeltaTime;
	}
}

void AOPAG_TornadoAbility::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AOPAG_Player* Player = Cast<AOPAG_Player>(OtherActor))
	{
		OverlappingTarget = Player;
	}	
}

void AOPAG_TornadoAbility::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AOPAG_Player* Player = Cast<AOPAG_Player>(OtherActor))
	{
		DamageTimer = 0;
		OverlappingTarget = nullptr;
	}
}

void AOPAG_TornadoAbility::Init(EAbilityTypes AbilityType, float Delay)
{
	Super::Init(AbilityType, Delay);
}

void AOPAG_TornadoAbility::DestroyAbility()
{
	AbilityCollider->SetCollisionProfileName(TEXT("NoCollision"));
	this->SetActorEnableCollision(false);

	Super::DestroyAbility();
}

void AOPAG_TornadoAbility::DealDamage(AActor* TargetActor)
{
	DamageTimer = DamageOverTimeFrequency;
	if (AOPAG_Player* Player = Cast<AOPAG_Player>(TargetActor))
	{
		Player->ReceiveDamage(AbilityDamage, this);
	}
}
