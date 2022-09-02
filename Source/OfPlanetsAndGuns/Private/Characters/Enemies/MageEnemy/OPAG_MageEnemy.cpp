// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/MageEnemy/OPAG_MageEnemy.h"
#include "Managers/OPAG_EventManagerSubsystem.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Abilities/OPAG_AbilityCasterComponent.h"
#include "Characters/Enemies/OPAG_AICBase.h"
#include "NavigationSystem.h"

AOPAG_MageEnemy::AOPAG_MageEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	AbilityCaster = CreateDefaultSubobject<UOPAG_AbilityCasterComponent>("AbilityCaster");	
}

void AOPAG_MageEnemy::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (EMS != nullptr)
	{
		EMS->EnemyKilled.AddDynamic(this,
			&AOPAG_MageEnemy::OnEnemyDeath);

		EMS->SpawnedEnemy.AddDynamic(this,
			&AOPAG_MageEnemy::OnSpawnedEnemy);

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

void AOPAG_MageEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bHealth50 && (GetHealth() * 100 / GetHealthMax() <= 50.f))
	{
		bHealth50 = true;

		if (AOPAG_AICBase* AIController = Cast<AOPAG_AICBase>(GetController()))
		{
			AIController->SetBlackboardEntryAsBool("bHealth50", true);
		}
	}

	if (!bHealth25 && (GetHealth() * 100 / GetHealthMax() <= 25.f))
	{
		bHealth25 = true;

		if (AOPAG_AICBase* AIController = Cast<AOPAG_AICBase>(GetController()))
		{
			AIController->SetBlackboardEntryAsBool("bHealth25", true);
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
	// when an enemy is killed, check if it's one of those casted by the mage, and if yes, untrack it by removing him from th array
	if (CastedEnemiesList.Contains(Enemy))
	{
		CastedEnemiesList.Remove(Enemy);

		if (CastedEnemiesList.Num() == 0)
		{
			if (AOPAG_AICBase* AIController = Cast<AOPAG_AICBase>(GetController()))
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Orange, TEXT("Enemy wave has been cleared"));

				AIController->SetBlackboardEntryAsBool("bNewWave", true);
				EMS->CastedEnemyWaveCleared.Broadcast();
			}
		}
	}	
}

void AOPAG_MageEnemy::OnSpawnedEnemy(AOPAG_Enemy* Enemy, AActor* Caller)
{
	// check if the Spawned Enemy ability was invoked by himself
	if (Caller == this)
	{
		CastedEnemiesList.Add(Enemy);
	}
}

void AOPAG_MageEnemy::OnBrokenForceField()
{
	FightStatus = EFightStatus::SecondPhase;

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
