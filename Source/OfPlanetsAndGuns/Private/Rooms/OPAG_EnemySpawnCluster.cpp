// Fill out your copyright notice in the Description page of Project Settings.


#include "Rooms/OPAG_EnemySpawnCluster.h"

#include "Components/BillboardComponent.h"
#include "Util/OPAG_UtilityFunctionLibrary.h"

// Sets default values
AOPAG_EnemySpawnCluster::AOPAG_EnemySpawnCluster()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	const auto SpriteFinder = ConstructorHelpers::FObjectFinder<UTexture2D>(
		TEXT("Texture2D'/Game/OfPlanetsAndGuns/UI/Billboards/EnemyClusterBillboard.EnemyClusterBillboard'"));
	if (SpriteFinder.Succeeded()) Billboard->SetSprite(SpriteFinder.Object);
}

// Called when the game starts or when spawned
void AOPAG_EnemySpawnCluster::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOPAG_EnemySpawnCluster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

int32 AOPAG_EnemySpawnCluster::GetSpawnerCount() const
{
	return Spawners.Num();
}

TArray<TSoftObjectPtr<AOPAG_Enemy>> AOPAG_EnemySpawnCluster::SpawnEnemies(
	TMap<TSubclassOf<AOPAG_Enemy>, float>& EnemyMap, int32 Amount) const
{
	TArray<TSoftObjectPtr<AOPAG_Enemy>> SpawnedEnemies;
	TArray<AOPAG_EnemySpawnPoint*> ShuffledSpawners = Spawners;
	UOPAG_UtilityFunctionLibrary::ShuffleArray(ShuffledSpawners);
	for (const TSoftObjectPtr<AOPAG_EnemySpawnPoint> Spawner : ShuffledSpawners)
	{
		if (Amount == 0) break;
		SpawnedEnemies.Append(Spawner->SpawnEnemies(EnemyMap, Amount));
		Amount--;
	}
	return SpawnedEnemies;
}

