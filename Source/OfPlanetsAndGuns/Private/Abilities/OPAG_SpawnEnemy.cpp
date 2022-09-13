// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/OPAG_SpawnEnemy.h"
#include "Managers\RoomManager\OPAG_EnemyLevel.h"
#include "Characters/Enemies/OPAG_Enemy.h"


void AOPAG_SpawnEnemy::Init(EAbilityTypes AbilityType, float Delay)
{
	Super::Init(AbilityType, Delay);
}

void AOPAG_SpawnEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (SpawnableEnemiesTable)
	{
		FString Context;
		SpawnableEnemiesTable->GetAllRows(Context, SpawnableEnemiesList);

		TSubclassOf<AOPAG_Enemy> EnemyClass;

		for (auto AbilityTypeToEnemy : SpawnableEnemiesList)
		{
			if (AbilityTypeToEnemy->AbilityType == this->CastedAbilityType)
			{
				EnemyClass = AbilityTypeToEnemy->EnemyClass;
				SpawnEnemy(EnemyClass);

				break;
			}
		}
	}
}

template <class T>
void AOPAG_SpawnEnemy::SpawnEnemy(T EnemyClass)
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AOPAG_Enemy* Enemy = GetWorld()->SpawnActor<AOPAG_Enemy>(EnemyClass, GetActorLocation(), GetActorLocation().ToOrientationRotator(), SpawnInfo);

	if (Enemy != nullptr)
		Enemy->SetActorRotation(FRotator::ZeroRotator);

	if (EMS != nullptr)
	{
		EMS->SpawnedEnemy.Broadcast(Enemy, Caller);
	}
}

void AOPAG_SpawnEnemy::DestroyAbility()
{
	this->SetActorEnableCollision(false);

	Super::DestroyAbility();
}