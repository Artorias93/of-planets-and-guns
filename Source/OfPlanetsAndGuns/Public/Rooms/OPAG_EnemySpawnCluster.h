// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OPAG_EnemySpawnPoint.h"
#include "GameFramework/Actor.h"
#include "OPAG_EnemySpawnCluster.generated.h"

UCLASS()
class OFPLANETSANDGUNS_API AOPAG_EnemySpawnCluster : public AOPAG_EnemySpawnPoint
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOPAG_EnemySpawnCluster();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<TSoftObjectPtr<AOPAG_EnemySpawnPoint>> Spawners;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual int32 GetSpawnerCount() const override;
	virtual TArray<TSoftObjectPtr<AOPAG_Enemy>> SpawnEnemies(
		TMap<TSoftClassPtr<AOPAG_Enemy>, float>& EnemyMap, int32 Amount) const override;

};
