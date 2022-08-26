// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/OPAG_SpawnEnemy.h"
#include "Managers\RoomManager\OPAG_EnemyLevel.h"
#include "Characters/Enemies/OPAG_Enemy.h"


void AOPAG_SpawnEnemy::Init(EAbilityTypes AbilityType)
{
	TSubclassOf<AOPAG_Enemy> EnemyClass;

	for (auto AbilityTypeToEnemy : SpawnableEnemiesList)
	{
		if (AbilityTypeToEnemy->AbilityType == AbilityType)
		{
			EnemyClass = AbilityTypeToEnemy->EnemyClass;
			SpawnEnemy(EnemyClass);
			break;
		}
	}	
}

void AOPAG_SpawnEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (SpawnableEnemiesTable)
	{
		FString Context;
		SpawnableEnemiesTable->GetAllRows(Context, SpawnableEnemiesList);
	}
}

template <class T>
void AOPAG_SpawnEnemy::SpawnEnemy(T EnemyClass)
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AOPAG_Enemy* Enemy = GetWorld()->SpawnActor<AOPAG_Enemy>(EnemyClass, GetActorLocation(), GetActorLocation().ToOrientationRotator(), SpawnInfo);
}
