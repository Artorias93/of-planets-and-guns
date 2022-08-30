// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/OPAG_SupernovaAbility.h"
#include "NiagaraComponent.h"
#include "Components/BoxComponent.h"
#include "Characters/Player/OPAG_Player.h"
#include "NiagaraFunctionLibrary.h"
#include "DrawDebugHelpers.h"

AOPAG_SupernovaAbility::AOPAG_SupernovaAbility()
{	
	PrimaryActorTick.bCanEverTick = true;

	AbilityCollider = CreateDefaultSubobject<UBoxComponent>("Collider");
	AbilityCollider->SetupAttachment(Pivot);

	AbilityParticleMuzzle = CreateDefaultSubobject<UNiagaraComponent>("Particle Muzzle");
	AbilityParticleMuzzle->SetupAttachment(AbilityCollider);
	AbilityParticleMuzzle->bAutoActivate = true;
}

void AOPAG_SupernovaAbility::BeginPlay()
{
	Super::BeginPlay();	

	AbilityCollider->OnComponentBeginOverlap.AddDynamic(this, &AOPAG_SupernovaAbility::OnOverlapBegin);

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AOPAG_SupernovaAbility::ShootProjectile, 2 + AbilityDelay, false);
}

void AOPAG_SupernovaAbility::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	

	if (bShoot && Target != nullptr)
	{
		FVector Velocity = MovementDirection * Speed * DeltaTime;
		FVector NewLocation = GetActorLocation() + Velocity;

		SetActorLocation(NewLocation);
	}
}

void AOPAG_SupernovaAbility::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AOPAG_Player* Player = Cast<AOPAG_Player>(OtherActor))
	{
		Player->ReceiveDamage(AbilityDamage);
	}

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), AbilityParticleHit, OtherActor->GetActorLocation(), GetActorRotation());
	AbilityCollider->SetCollisionProfileName(TEXT("NoCollision"));
	this->SetActorEnableCollision(false);

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AOPAG_SupernovaAbility::DestroyAbility, 2, false);
}

void AOPAG_SupernovaAbility::ShootProjectile()
{
	bShoot = true;

	TargetLocation = Target->GetActorLocation();
	MovementDirection = Target->GetActorLocation() - this->GetActorLocation();
	MovementDirection.Normalize();

	UNiagaraFunctionLibrary::SpawnSystemAttached(AbilityParticleProjectile, AbilityCollider, "", GetActorLocation(), GetActorRotation(), EAttachLocation::KeepWorldPosition, false);
}

void AOPAG_SupernovaAbility::Init(EAbilityTypes AbilityType, float Delay)
{
	Super::Init(AbilityType, Delay);
}

void AOPAG_SupernovaAbility::DestroyAbility()
{
	AbilityCollider->SetCollisionProfileName(TEXT("NoCollision"));
	this->SetActorEnableCollision(false);

	Super::DestroyAbility();
}
