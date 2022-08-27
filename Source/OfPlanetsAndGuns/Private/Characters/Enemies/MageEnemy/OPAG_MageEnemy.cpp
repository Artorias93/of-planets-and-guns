// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/MageEnemy/OPAG_MageEnemy.h"
#include "Characters/Enemies/PillarEnemy/OPAG_PillarEnemy.h"
#include "Components/ChildActorComponent.h"
#include "Managers/OPAG_EventManagerSubsystem.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Abilities/OPAG_AbilityCaster.h"
#include "Abilities/OPAG_ForceField.h"

AOPAG_MageEnemy::AOPAG_MageEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AOPAG_MageEnemy::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// Check if already exists an Ability Caster instance, if yes use it, otherwise create it
	AActor* AbilityCasterActor = UGameplayStatics::GetActorOfClass(GetWorld(), AOPAG_AbilityCaster::StaticClass());
	if (AbilityCasterActor != nullptr)
	{
		AbilityCaster = Cast<AOPAG_AbilityCaster>(AbilityCasterActor);
	}
	else
		AbilityCaster = GetWorld()->SpawnActor<AOPAG_AbilityCaster>(AOPAG_AbilityCaster::StaticClass(), this->GetActorLocation(), this->GetActorRotation(), SpawnInfo);
	
	// retrieve all pillars in game. The amount will determine when the first phase will finish
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AOPAG_PillarEnemy::StaticClass(), Pillars);
	AvailablePillarsAmount = Pillars.Num();

	// if no pillars are already spawned, then the enemy will go direcetly in second phase
	if (AvailablePillarsAmount <= 0)
		FightStatus = FIGHT_STATUS::SECOND;

	if (EMS != nullptr)
	{
		EMS->EnemyKilled.AddDynamic(this,
			&AOPAG_MageEnemy::OnEnemyDeath);

		// the check is mandatory even if we don't use it directly, since the Ability Caster will receive the event
		if (AbilityCaster != nullptr)
		{			
			FVector Location = GetActorLocation();
			Location.Z -= 300;

			EMS->CastAbility.Broadcast(AOPAG_ForceField::StaticClass(), this, Location, ForceField);
		}
	}

	
}

void AOPAG_MageEnemy::Attack()
{

}

void AOPAG_MageEnemy::Death()
{

}

void AOPAG_MageEnemy::OnEnemyDeath(const AOPAG_Enemy* const Enemy)
{
	
	if (const AOPAG_PillarEnemy* Pillar = Cast<AOPAG_PillarEnemy>(Enemy))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Pillar destroyed"));
		DestroyedPillarCounter++;

		// if all pillars are destroyed, then will start the second phase
		if (DestroyedPillarCounter >= AvailablePillarsAmount)
		{
			FightStatus = FIGHT_STATUS::SECOND;
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Second phase has begun"));
		}
	}
}
