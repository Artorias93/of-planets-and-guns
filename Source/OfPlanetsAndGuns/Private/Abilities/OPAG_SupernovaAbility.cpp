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
	AbilityCollider->SetupAttachment(RootComponent);

	AbilityParticleMuzzle = CreateDefaultSubobject<UNiagaraComponent>("Particle Muzzle");
	AbilityParticleMuzzle->SetupAttachment(AbilityCollider);
	AbilityParticleMuzzle->bAutoActivate = true;
}

void AOPAG_SupernovaAbility::BeginPlay()
{
	Super::BeginPlay();

	AbilityCollider->OnComponentBeginOverlap.AddDynamic(this, &AOPAG_SupernovaAbility::OnOverlapBegin);

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AOPAG_SupernovaAbility::ShootProjectile, 2, false);

}

void AOPAG_SupernovaAbility::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("ticking"));
	if (bShoot)
	{
		FVector CurrentLocation = this->GetActorLocation();

		CurrentLocation.X += speed * DeltaTime; // use X Y or Z.  Use -= to go the opposite way
		CurrentLocation.Z -= speed/2 * DeltaTime; // use X Y or Z.  Use -= to go the opposite way

		SetActorLocation(CurrentLocation);
	}
}

void AOPAG_SupernovaAbility::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AOPAG_CharacterBase* Character = Cast<AOPAG_CharacterBase>(OtherActor))
	{
		Character->ReceiveDamage(50);
	}

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), AbilityParticleHit, OtherActor->GetActorLocation(), GetActorRotation());
}

void AOPAG_SupernovaAbility::ShootProjectile()
{
	bShoot = true;
	//UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), AbilityParticleProjectile, GetActorLocation(), GetActorRotation());
	UNiagaraFunctionLibrary::SpawnSystemAttached(AbilityParticleProjectile, AbilityCollider, "", GetActorLocation(), GetActorRotation(), EAttachLocation::KeepWorldPosition, false);
}

void AOPAG_SupernovaAbility::Init(EAbilityTypes AbilityType)
{

}
