// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/OPAG_SupernovaAbility.h"
#include "NiagaraComponent.h"
#include "Components/BoxComponent.h"
#include "Characters/OPAG_CharacterBase.h"
#include "NiagaraFunctionLibrary.h"

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
		FVector CurrentLocation = GetActorLocation();
		FVector MovementVector = TargetLocation - CurrentLocation;
		FVector Velocity{ MovementVector.Normalize() * Speed * DeltaTime };
		FVector NewLocation = CurrentLocation + Velocity;

		SetActorLocation(NewLocation);
	}
}

void AOPAG_SupernovaAbility::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AOPAG_CharacterBase* Character = Cast<AOPAG_CharacterBase>(OtherActor))
	{
		Character->ReceiveDamage(AbilityDamage);
	}

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), AbilityParticleHit, OtherActor->GetActorLocation(), GetActorRotation());
	AbilityCollider->SetCollisionProfileName(TEXT("NoCollision"));
	this->SetActorEnableCollision(false);
}

void AOPAG_SupernovaAbility::ShootProjectile()
{
	bShoot = true;
	//UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), AbilityParticleProjectile, GetActorLocation(), GetActorRotation());
	TargetLocation = Target->GetActorLocation();
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
