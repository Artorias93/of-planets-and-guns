// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/MageEnemy/OPAG_MageEnemy.h"
#include "Components/ChildActorComponent.h"
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

	// since the force field has been broken, enable again the collisions
	GetMesh()->SetCollisionProfileName(TEXT("BlockAll"));
	this->SetActorEnableCollision(true);

	FVector TornadoPosition = GetRandomPointInRadius();
	TornadoPosition.Z += 250;

	FVector SupernovaPosition = GetActorLocation();
	SupernovaPosition.Z += 500;

	/*EMS->CastAbility.Broadcast(EAbilityTypes::Spawn_ShieldEnemy, this, GetRandomPointInRadius());
	EMS->CastAbility.Broadcast(EAbilityTypes::Tornado, this, TornadoPosition);
	EMS->CastAbility.Broadcast(EAbilityTypes::Crack, this, GetRandomPointInRadius());
	EMS->CastAbility.Broadcast(EAbilityTypes::Ray, this, GetActorLocation());*/
	EMS->CastAbility.Broadcast(EAbilityTypes::Supernova, this, SupernovaPosition);
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
