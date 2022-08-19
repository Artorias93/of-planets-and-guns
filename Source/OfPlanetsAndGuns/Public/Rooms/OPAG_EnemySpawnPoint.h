// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OPAG_EnemySpawnPoint.generated.h"

class UBillboardComponent;
class AOPAG_Enemy;
UCLASS()
class OFPLANETSANDGUNS_API AOPAG_EnemySpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOPAG_EnemySpawnPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBillboardComponent* Billboard;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		virtual TArray<TSoftObjectPtr<AOPAG_Enemy>> SpawnEnemies(
			TMap<TSubclassOf<AOPAG_Enemy>, float>& EnemyMap, int32 Amount) const;

	UFUNCTION(BlueprintCallable)
		virtual int32 GetSpawnerCount() const;

};
