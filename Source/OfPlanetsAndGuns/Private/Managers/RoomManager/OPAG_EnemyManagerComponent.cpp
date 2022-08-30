// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/RoomManager/OPAG_EnemyManagerComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Managers/OPAG_EventManagerSubsystem.h"
#include "Managers/OPAG_GameInstance.h"
#include "Managers/RoomManager/OPAG_EnemyLevel.h"
#include "Managers/RoomManager/OPAG_RoomManager.h"
#include "Rooms/OPAG_EnemySpawnPoint.h"
#include "Util/OPAG_UtilityFunctionLibrary.h"

// Sets default values for this component's properties
UOPAG_EnemyManagerComponent::UOPAG_EnemyManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SpawnMode = ESpawnMode::MultipleWaves;
	SpawningPercentage = 100.f;
	// ...
}


void UOPAG_EnemyManagerComponent::LoadEnemyLevelData()
{
	const FString Context;
	if (EnemyTypes && BaseEnemyLevels.Num() == 0) EnemyTypes->GetAllRows(Context, BaseEnemyLevels);
	if (EnemySpawnsPerLevelTable && EnemySpawnsPerLevel.Num() == 0)
		EnemySpawnsPerLevelTable->GetAllRows(Context, EnemySpawnsPerLevel);

}

int32 UOPAG_EnemyManagerComponent::GetSpawnerCount() const
{
	int32 SpawnerCount = 0;
	for (const TSoftObjectPtr<AOPAG_EnemySpawnPoint> Spawner : Spawners)
	{
		SpawnerCount += Spawner->GetSpawnerCount();
	}
	return SpawnerCount;
}

int32 UOPAG_EnemyManagerComponent::GetEnemyCountToSpawn(const float Difficulty, const AOPAG_RoomManager* RoomManager) const
{
	if (EnemySpawnsPerLevel.Num() == 0) return GetSpawnerCount()* FMath::RoundToInt(Difficulty * 2.f);

	const FOPAG_EnemySpawnsPerLevel* ClosestMatch = EnemySpawnsPerLevel[0];
	for (const FOPAG_EnemySpawnsPerLevel* SpawnsPerLevel : EnemySpawnsPerLevel)
	{
		if (FMath::Abs(Difficulty - SpawnsPerLevel->Level) < FMath::Abs(Difficulty - ClosestMatch->Level))
			ClosestMatch = SpawnsPerLevel;
	}

	const int RoomSize = RoomManager->Weight;
	switch (RoomSize)
	{
	case 3: return ClosestMatch->LargeRoomSpawns;
	case 2: return ClosestMatch->MediumRoomSpawns;
	default: return ClosestMatch->SmallRoomSpawns;
	}
}

void UOPAG_EnemyManagerComponent::GetBaseMappedEnemies(TMap<TSubclassOf<AOPAG_Enemy>, float>& EnemyProbabilityMap)
{
	for (const FOPAG_EnemyLevel* BaseEnemyLevel : BaseEnemyLevels)
	{
		EnemyProbabilityMap.Add(BaseEnemyLevel->Enemy, BaseEnemyLevel->Level);
	}
}

void UOPAG_EnemyManagerComponent::OnEnemyDeath(const AOPAG_Enemy* const Enemy)
{
	if (EnemiesInRoom.Remove(Enemy) > 0)
	{
		EnemyDeathCount++;
		if (EnemyDeathCount >= TotalEnemyAmount)
		{
			AOPAG_RoomManager* RoomManager = GetOwner<AOPAG_RoomManager>();
			if (RoomManager)
			{
				RoomManager->RoomCleared();
			}
		}
		else
		{
			SpawnWave();
		}
	}
}

	// Called when the game starts
void UOPAG_EnemyManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	const UOPAG_GameInstance* GI = Cast<UOPAG_GameInstance>(
		UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI != nullptr)
	{
		GI->GetEventManager()->EnemyKilled.AddDynamic(this, 
			&UOPAG_EnemyManagerComponent::OnEnemyDeath);
	}
}

void UOPAG_EnemyManagerComponent::InitializeComponent()
{
	Super::InitializeComponent();
	LoadEnemyLevelData();
}


// Called every frame
void UOPAG_EnemyManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, 
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UOPAG_EnemyManagerComponent::SpawnWave()
{
	LoadEnemyLevelData();

	const AOPAG_RoomManager* RoomManager = GetOwner<AOPAG_RoomManager>();
	checkf(RoomManager, TEXT("Bad Room Manager Owner"));

	const float Difficulty = RoomManager->GetCurrentRoomDifficulty();

	if (TotalEnemyAmount == 0)
	{
		TotalEnemyAmount = GetEnemyCountToSpawn(Difficulty, RoomManager);
	}
	if (EnemiesInRoom.Num() == 0)
	{
		int32 EnemiesToSpawnNextWave = FMath::RoundToInt(
			GetSpawnerCount() * (SpawningPercentage * 0.01f));
		if (EnemiesToSpawnNextWave > TotalEnemyAmount - EnemyDeathCount)
			EnemiesToSpawnNextWave = TotalEnemyAmount - EnemyDeathCount;

		// if single wave mode, adjust total enemies to not spawn a second wave
		if (SpawnMode == ESpawnMode::SingleWave) TotalEnemyAmount = EnemiesToSpawnNextWave;

		UOPAG_UtilityFunctionLibrary::ShuffleArray(Spawners);

		TMap<TSubclassOf<AOPAG_Enemy>, float> EnemyProbabilityMap;
		GetBaseMappedEnemies(EnemyProbabilityMap);
		UOPAG_UtilityFunctionLibrary::MapBaseItemsWithDifficulty(EnemyProbabilityMap, Difficulty);

		for (int i = 0; i < SpawnAttempts && EnemiesToSpawnNextWave > 0; ++i)
		{
			for (const TSoftObjectPtr<AOPAG_EnemySpawnPoint> Spawner : Spawners)
			{
				if (EnemiesToSpawnNextWave <= 0)
				{
					break;
				}

				TArray<TSoftObjectPtr<AOPAG_Enemy>> SpawnedEnemies =
					Spawner->SpawnEnemies(EnemyProbabilityMap, EnemiesToSpawnNextWave);

				EnemiesToSpawnNextWave -= SpawnedEnemies.Num();
				EnemiesInRoom.Append(SpawnedEnemies);
			}
		}
	}
}

