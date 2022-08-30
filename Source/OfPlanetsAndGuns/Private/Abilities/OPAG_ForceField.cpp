// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/OPAG_ForceField.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include <Engine.h>
#include "Characters/Enemies/PillarEnemy/OPAG_PillarEnemy.h"

AOPAG_ForceField::AOPAG_ForceField()
{
	AbilityMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	AbilityMesh->SetupAttachment(Pivot);
}

void AOPAG_ForceField::BeginPlay()
{
	Super::BeginPlay();

	if (EMS != nullptr)
	{
		EMS->DestroyedPillar.AddDynamic(this, &AOPAG_ForceField::OnPillarDestroyed);
	}

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AOPAG_PillarEnemy::StaticClass(), Pillars);
	AvailablePillarsAmount = Pillars.Num();

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("%s"),  AvailablePillarsAmount));
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