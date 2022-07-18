// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OPAG_EnemyManagerComponent.generated.h"


class UDataTable;
struct FOPAG_EnemyLevel;
class AOPAG_Enemy;
class AOPAG_EnemySpawnPoint;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OFPLANETSANDGUNS_API UOPAG_EnemyManagerComponent : public UActorComponent
{
	GENERATED_BODY()

	TArray<TSoftObjectPtr<AOPAG_Enemy>> EnemiesInRoom;

	TArray<FOPAG_EnemyLevel*> BaseEnemyLevels;
	int EnemyDeathCount;

public:	
	// Sets default values for this component's properties
	UOPAG_EnemyManagerComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Manager")
		TArray<TSoftObjectPtr<AOPAG_EnemySpawnPoint>> Spawners;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Manager")
		UDataTable* EnemyTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Manager")
		float Spread;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Manager")
		int TotalEnemyAmount;

private:

	float GaussianProbabilityModifier(float Level) const;

	int32 GetSpawnerCount();

	void GetMappedEnemies(TMap<TSoftClassPtr<AOPAG_Enemy>, float>& EnemyProbabilityMap);

	UFUNCTION()
		void OnEnemyDeath(AOPAG_Enemy* Enemy);

	template<typename T>
	static void ShuffleArray(TArray<T>& Array);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* 
		ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Enemy Manager")
		void SpawnWave();
		
};
