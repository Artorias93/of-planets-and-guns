// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/OPAG_FireballAbility.h"
#include "Components/BoxComponent.h"
#include "Characters/Player/OPAG_Player.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"

AOPAG_FireballAbility::AOPAG_FireballAbility()
{	
	PrimaryActorTick.bCanEverTick = true;

	AbilityCollider = CreateDefaultSubobject<UBoxComponent>("Collider");
	AbilityCollider->SetupAttachment(Pivot);

	AbilityParticleMuzzle = CreateDefaultSubobject<UParticleSystemComponent>("Particle Muzzle");
	AbilityParticleMuzzle->SetupAttachment(AbilityCollider);
	AbilityParticleMuzzle->bAutoActivate = true;	
}

void AOPAG_FireballAbility::BeginPlay()
{
	Super::BeginPlay();

	AbilityCollider->OnComponentBeginOverlap.AddDynamic(this, &AOPAG_FireballAbility::OnOverlapBegin);

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AOPAG_FireballAbility::ShootProjectile, 0.5 + AbilityDelay, false);
}

void AOPAG_FireballAbility::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bShoot && Target != nullptr)
	{		
		FVector Velocity = MovementDirection * Speed * DeltaTime;
		FVector NewLocation = GetActorLocation() + Velocity;

		SetActorLocation(NewLocation);
	}
}

void AOPAG_FireballAbility::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AOPAG_Player* Player = Cast<AOPAG_Player>(OtherActor))
	{
		Player->ReceiveDamage(AbilityDamage);
	}

	UGameplayStatics::SpawnEmitterAttached(
		AbilityParticleHit,
		AbilityCollider,
		NAME_None,
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		EAttachLocation::KeepRelativeOffset,
		true,
		EPSCPoolMethod::None,
		true
	);
	
	AbilityCollider->SetCollisionProfileName(TEXT("NoCollision"));
	this->SetActorEnableCollision(false);


	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AOPAG_FireballAbility::DestroyAbility, 0.5, false);
}

void AOPAG_FireballAbility::ShootProjectile()
{
	bShoot = true;
	//UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), AbilityParticleProjectile, GetActorLocation(), GetActorRotation());
	TargetLocation = Target->GetActorLocation();
	MovementDirection = Target->GetActorLocation() - this->GetActorLocation();
	MovementDirection.Normalize();
	
	UGameplayStatics::SpawnEmitterAttached(
		AbilityParticleProjectile,
		AbilityCollider,
		NAME_None,
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		EAttachLocation::KeepRelativeOffset,
		true,
		EPSCPoolMethod::None,
		true
	);
}

void AOPAG_FireballAbility::Init(EAbilityTypes AbilityType, float Delay)
{
	Super::Init(AbilityType, Delay);
}

void AOPAG_FireballAbility::DestroyAbility()
{
	AbilityCollider->SetCollisionProfileName(TEXT("NoCollision"));
	this->SetActorEnableCollision(false);

	Super::DestroyAbility();
}