// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/OPAG_AbilityBase.h"
#include "OPAG_SpawnEnemy.generated.h"

UCLASS()
class OFPLANETSANDGUNS_API AOPAG_SpawnEnemy : public AOPAG_AbilityBase
{
	GENERATED_BODY()

public:
	virtual void Init(EAbilityTypes AbilityType) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UDataTable* SpawnableEnemiesTable;

protected:
	virtual void BeginPlay() override;

	virtual void DestroyAbility() override;

	
	TArray<struct FOPAG_AbilityTypeToEnemyMapping*> SpawnableEnemiesList;

	template<class T>
	void SpawnEnemy(T EnemyClass);
};
