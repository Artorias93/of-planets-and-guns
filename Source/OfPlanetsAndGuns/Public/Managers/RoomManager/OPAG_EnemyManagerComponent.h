// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OPAG_EnemyManagerComponent.generated.h"


class UDataTable;
struct FOPAG_EnemyLevel;
class AOPAG_Enemy;
class AOPAG_EnemySpawnPoint;

UENUM(BlueprintType)
enum class ESpawnMode : uint8
{
	SingleWave,
	MultipleWaves
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OFPLANETSANDGUNS_API UOPAG_EnemyManagerComponent : public UActorComponent
{
	GENERATED_BODY()

	TArray<TSoftObjectPtr<AOPAG_Enemy>> EnemiesInRoom;

	TArray<FOPAG_EnemyLevel*> BaseEnemyLevels;
	int32 EnemyDeathCount;

public:	
	// Sets default values for this component's properties
	UOPAG_EnemyManagerComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Manager")
		ESpawnMode SpawnMode;

	//Percentage of how many spawners will be spawning out of the list of spawners
	//(Eg: 70% will use 7/10 available spawners to spawn the wave)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Manager", meta = (UIMin = 1.f, UIMax = 100.f))
		float SpawningPercentage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Manager")
		TArray<AOPAG_EnemySpawnPoint*> Spawners;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Manager")
		UDataTable* EnemyTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Manager")
		float Spread;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Manager")
		int32 TotalEnemyAmount;

private:

	void LoadEnemyLevelData();

	float GaussianProbabilityModifier(float Level) const;

	void GetMappedEnemies(TMap<TSoftClassPtr<AOPAG_Enemy>, float>& EnemyProbabilityMap);

	UFUNCTION()
		void OnEnemyDeath(const AOPAG_Enemy* const Enemy);
	

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void InitializeComponent() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* 
		ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Enemy Manager")
		void SpawnWave();

	int32 GetSpawnerCount() const;

};
