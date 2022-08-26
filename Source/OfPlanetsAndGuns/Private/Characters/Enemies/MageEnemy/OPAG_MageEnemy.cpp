// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/MageEnemy/OPAG_MageEnemy.h"
#include "Components/ChildActorComponent.h"
#include "Managers/OPAG_EventManagerSubsystem.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Abilities/OPAG_AbilityCasterComponent.h"

AOPAG_MageEnemy::AOPAG_MageEnemy()
{
	PrimaryActorTick.bCanEverTick = false;

	AbilityCaster = CreateDefaultSubobject<UOPAG_AbilityCasterComponent>("AbilityCaster");
}

void AOPAG_MageEnemy::BeginPlay()
{
	Super::BeginPlay();

	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	this->SetActorEnableCollision(false);

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (EMS != nullptr)
	{
		EMS->EnemyKilled.AddDynamic(this,
			&AOPAG_MageEnemy::OnEnemyDeath);

		EMS->BrokenForceField.AddDynamic(this,
			&AOPAG_MageEnemy::OnBrokenForceField);

		// the check is mandatory even if we don't use it directly, since the Ability Caster will receive the event
		if (AbilityCaster != nullptr)
		{			
			FVector UpRightCorner = GetActorLocation();
			UpRightCorner.X += 750;
			UpRightCorner.Y += 750;
			FVector UpLeftCorner = GetActorLocation();
			UpLeftCorner.X += 750;
			UpLeftCorner.Y -= 750;
			FVector DownRightCorner = GetActorLocation();
			DownRightCorner.X -= 750;
			DownRightCorner.Y += 750;
			FVector DownLeftCorner = GetActorLocation();
			DownLeftCorner.X -= 750;
			DownLeftCorner.Y -= 750;

			EMS->CastAbility.Broadcast(EAbilityTypes::Spawn_PillarEnemy, this, UpRightCorner);
			EMS->CastAbility.Broadcast(EAbilityTypes::Spawn_PillarEnemy, this, UpLeftCorner);
			EMS->CastAbility.Broadcast(EAbilityTypes::Spawn_PillarEnemy, this, DownRightCorner);
			EMS->CastAbility.Broadcast(EAbilityTypes::Spawn_PillarEnemy, this, DownLeftCorner);

			EMS->CastAbility.Broadcast(EAbilityTypes::ForceField, this, GetActorLocation());
		}
	}	
}

void AOPAG_MageEnemy::Attack()
{

}

void AOPAG_MageEnemy::Death()
{
	Super::Death();
}

void AOPAG_MageEnemy::OnEnemyDeath(const AOPAG_Enemy* const Enemy)
{

}

void AOPAG_MageEnemy::OnBrokenForceField()
{
	FightStatus = EFightStatus::SecondPhase;
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Second Stage started"));	

	GetMesh()->SetCollisionProfileName(TEXT("BlockAll"));
	this->SetActorEnableCollision(true);

	EMS->CastAbility.Broadcast(EAbilityTypes::Spawn_ShieldEnemy, this, GetActorLocation());
}
