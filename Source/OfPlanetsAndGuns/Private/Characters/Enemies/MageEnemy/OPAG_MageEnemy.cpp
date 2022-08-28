// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/MageEnemy/OPAG_MageEnemy.h"
#include "Managers/OPAG_EventManagerSubsystem.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Abilities/OPAG_AbilityCasterComponent.h"
#include "Characters/Enemies/OPAG_AICBase.h"
#include "NavigationSystem.h"

AOPAG_MageEnemy::AOPAG_MageEnemy()
{
	PrimaryActorTick.bCanEverTick = false;

	AbilityCaster = CreateDefaultSubobject<UOPAG_AbilityCasterComponent>("AbilityCaster");	
}

void AOPAG_MageEnemy::BeginPlay()
{
	Super::BeginPlay();

	// disable collision in order to let the enemy be positioned inside the force field that he will cast at run time
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
			// Spawn pillars and force field
			Init();
		}
	}	
}

void AOPAG_MageEnemy::CastAbilityRequest(EAbilityTypes AbilityType, AActor* Caller, AActor* Target, EAbilitySpawnPattern AbilitySpawnPattern)
{
	EMS->CastAbility.Broadcast(AbilityType, Caller, Target, AbilitySpawnPattern);
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

	// since the force field has been broken, enable again the collisions
	GetMesh()->SetCollisionProfileName(TEXT("BlockAll"));
	this->SetActorEnableCollision(true);

	// change Blackboard value in order to change BT selection

	if (AOPAG_AICBase* AIController = Cast<AOPAG_AICBase>(GetController()))
	{
		AIController->SetBlackboardEntryAsBool("bSecondStage", true);
	}

}

FVector AOPAG_MageEnemy::GetRandomPointInRadius()
{
	if (AOPAG_AICBase* const AIController = Cast<AOPAG_AICBase>(GetController()))
	{
		if (const AOPAG_Enemy* const Enemy = Cast<AOPAG_Enemy>(AIController->GetPawn()))
		{
			const FVector ActorLocation = Enemy->GetActorLocation();
			if (const UNavigationSystemV1* const NavSystem = UNavigationSystemV1::GetCurrent(GetWorld()))
			{
				//Gets the random point and writes it on the blackboard
				FNavLocation NewLocation;
				if (NavSystem->GetRandomPointInNavigableRadius(GetActorLocation(), 1000.f, NewLocation))
				{
					return NewLocation.Location;
				}
			}
		}
	}
	return GetActorLocation();
}

AActor* AOPAG_MageEnemy::GetFocusedActor() const
{
	if (AOPAG_AICBase* const AIController = Cast<AOPAG_AICBase>(GetController()))
	{
		return AIController->GetFocusActor();
	}

	return nullptr;
}

void AOPAG_MageEnemy::Init()
{
	CastAbilityRequest(EAbilityTypes::Spawn_PillarEnemy, this, nullptr, EAbilitySpawnPattern::SingleBackLeftCorner);
	CastAbilityRequest(EAbilityTypes::Spawn_PillarEnemy, this, nullptr,EAbilitySpawnPattern::SingleBackRightCorner);
	CastAbilityRequest(EAbilityTypes::Spawn_PillarEnemy, this, nullptr, EAbilitySpawnPattern::SingleFrontLeftCorner);
	CastAbilityRequest(EAbilityTypes::Spawn_PillarEnemy, this, nullptr,EAbilitySpawnPattern::SingleFrontRightCorner);

	CastAbilityRequest(EAbilityTypes::ForceField, this, nullptr, EAbilitySpawnPattern::Simple);
}

float AOPAG_MageEnemy::GetGroundHeight()
{
	return GetRandomPointInRadius().Z;
}
