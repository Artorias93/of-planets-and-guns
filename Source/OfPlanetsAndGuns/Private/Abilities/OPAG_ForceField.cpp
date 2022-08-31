// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/OPAG_ForceField.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include <Engine.h>
#include "Components/SphereComponent.h"
#include "Characters/Enemies/PillarEnemy/OPAG_PillarEnemy.h"

AOPAG_ForceField::AOPAG_ForceField()
{
	PrimaryActorTick.bCanEverTick = true;

	AbilityCollider = CreateDefaultSubobject<USphereComponent>("Collider");
	AbilityCollider->SetupAttachment(Pivot);

	AbilityMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	AbilityMesh->SetupAttachment(AbilityCollider);
}

void AOPAG_ForceField::BeginPlay()
{
	Super::BeginPlay();

	if (EMS != nullptr)
	{
		EMS->DestroyedPillar.AddDynamic(this, &AOPAG_ForceField::OnPillarDestroyed);
		EMS->CastedEnemyWaveCleared.AddDynamic(this, &AOPAG_ForceField::DestroyAbility);
	}

	AbilityCollider->OnComponentBeginOverlap.AddDynamic(this, &AOPAG_ForceField::OnOverlapBegin);

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AOPAG_PillarEnemy::StaticClass(), Pillars);
	AvailablePillarsAmount = Pillars.Num();	
}

void AOPAG_ForceField::Init(EAbilityTypes AbilityType, float Delay)
{
	Super::Init(AbilityType, Delay);
	
	if (Caller != nullptr)
	{		
		Caller->SetActorLocation(this->GetActorLocation(), false, nullptr, ETeleportType::TeleportPhysics);
	}
}

void AOPAG_ForceField::OnPillarDestroyed()
{
	AvailablePillarsAmount--;

	if (AvailablePillarsAmount <= 0 && EMS != nullptr)
	{
		EMS->BrokenForceField.Broadcast();
		Destroy();
	}

}

void AOPAG_ForceField::DestroyAbility()
{
	AbilityMesh->SetCollisionProfileName(TEXT("NoCollision"));
	this->SetActorEnableCollision(false);

	Super::DestroyAbility();
}

void AOPAG_ForceField::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	/*AbilityMesh->SetMaterial(0, ForceFieldHitMaterial);

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AOPAG_ForceField::ResetMaterial, 0.25f, false);*/
}

void AOPAG_ForceField::ResetMaterial()
{
	AbilityMesh->SetMaterial(0, ForceFieldMaterial);
}

void AOPAG_ForceField::GetHit()
{
	AbilityMesh->SetMaterial(0, ForceFieldHitMaterial);

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AOPAG_ForceField::ResetMaterial, 0.25f, false); 
}