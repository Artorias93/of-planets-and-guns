// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OPAG_EnemySpawnPoint.generated.h"

class AOPAG_Enemy;
UCLASS()
class OFPLANETSANDGUNS_API AOPAG_EnemySpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOPAG_EnemySpawnPoint();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBillboardComponent* Billboard;

private:
	static TSoftClassPtr<AOPAG_Enemy> GetRandomEnemy(TMap<TSoftClassPtr<AOPAG_Enemy>, float>& EnemyMap);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		virtual TArray<TSoftObjectPtr<AOPAG_Enemy>> SpawnEnemies(
			TMap<TSoftClassPtr<AOPAG_Enemy>, float>& EnemyMap, int32 Amount) const;

	UFUNCTION(BlueprintCallable)
		virtual int32 GetSpawnerCount() const;

};
