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

}

float UOPAG_EnemyManagerComponent::GaussianProbabilityModifier(const float Level) const
{
	UOPAG_GameInstance* GI = Cast<UOPAG_GameInstance>(
		UGameplayStatics::GetGameInstance(GetWorld()));
	float ModifiedLevel = GI->GetStageDifficulty() - Level;
	ModifiedLevel /= Spread;
	return FMath::Exp(FMath::Pow(ModifiedLevel, 2.f) * -0.5f);
}

int32 UOPAG_EnemyManagerComponent::GetSpawnerCount() const
{
	int32 SpawnCount = 0;
	for (const TSoftObjectPtr<AOPAG_EnemySpawnPoint> Spawner : Spawners)
	{
		SpawnCount += Spawner->GetSpawnerCount();
	}
	return SpawnCount;
}

void UOPAG_EnemyManagerComponent::GetMappedEnemies(TMap<TSoftClassPtr<AOPAG_Enemy>, float>& EnemyProbabilityMap)
{
	float OverallProbability = 0;
	TArray<TSoftClassPtr<AOPAG_Enemy>> Keys;

	LoadEnemyLevelData();
	for (const FOPAG_EnemyLevel* BaseEnemyLevel : BaseEnemyLevels)
	{
		float Probability = GaussianProbabilityModifier(BaseEnemyLevel->Level);
		OverallProbability += Probability;
		TSoftClassPtr<AOPAG_Enemy> Enemy = BaseEnemyLevel->Enemy;
		EnemyProbabilityMap.Add(Enemy, Probability);
		Keys.Add(Enemy);
	}

	for (TSoftClassPtr<AOPAG_Enemy> Enemy : Keys)
	{
		EnemyProbabilityMap[Enemy] /= OverallProbability;
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
	if (TotalEnemyAmount == 0)
	{
		UOPAG_GameInstance* GI = Cast<UOPAG_GameInstance>(
			UGameplayStatics::GetGameInstance(GetWorld()));
		TotalEnemyAmount = GetSpawnerCount() * FMath::RoundToInt(GI->GetStageDifficulty() * 2.f);
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

		TMap<TSoftClassPtr<AOPAG_Enemy>, float> EnemyProbabilityMap;
		GetMappedEnemies(EnemyProbabilityMap);
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

