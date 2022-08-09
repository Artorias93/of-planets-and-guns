// Fill out your copyright notice in the Description page of Project Settings.


#include "Rooms/OPAG_EnemySpawnPoint.h"

#include "Characters/Enemies/OPAG_Enemy.h"
#include "Components/BillboardComponent.h"


// Sets default values
AOPAG_EnemySpawnPoint::AOPAG_EnemySpawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Gizmo"));
	RootComponent = Billboard;
	Billboard->bIsScreenSizeScaled = true;
	Billboard->ScreenSize = -0.002f;
}

TSoftClassPtr<AOPAG_Enemy> AOPAG_EnemySpawnPoint::GetRandomEnemy(TMap<TSoftClassPtr<AOPAG_Enemy>, float>& EnemyMap)
{
	const float RandomChance = FMath::FRandRange(0.f, 1.f);
	float Sum = 0.f;
	TArray<TSoftClassPtr<AOPAG_Enemy>> Keys;
	EnemyMap.GetKeys(Keys);
	TSoftClassPtr<AOPAG_Enemy> ChosenEnemy = Keys.Last();
	for (TSoftClassPtr<AOPAG_Enemy> Key : Keys)
	{
		Sum += EnemyMap[Key];
		if (RandomChance < Sum)
		{
			ChosenEnemy = Key;
			break;
		}
	}
	return ChosenEnemy;
}

// Called when the game starts or when spawned
void AOPAG_EnemySpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOPAG_EnemySpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

TArray<TSoftObjectPtr<AOPAG_Enemy>> AOPAG_EnemySpawnPoint::SpawnEnemies(
	TMap<TSoftClassPtr<AOPAG_Enemy>, float>& EnemyMap, int32 Amount) const
{
	const TSoftClassPtr<AOPAG_Enemy> EnemyToSpawn = GetRandomEnemy(EnemyMap);
	const FActorSpawnParameters SpawnInfo;
	TArray<TSoftObjectPtr<AOPAG_Enemy>> SpawnedEnemies;
	TSoftObjectPtr<AOPAG_Enemy> Enemy = GetWorld()->SpawnActor<AOPAG_Enemy>(EnemyToSpawn.Get(), 
		GetActorLocation(), FRotator::ZeroRotator, SpawnInfo);
	if (Enemy.IsValid())
	{
		SpawnedEnemies.Add(Enemy);
	}
	return SpawnedEnemies;
}

int32 AOPAG_EnemySpawnPoint::GetSpawnerCount() const
{
	return 1;
}

