// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "OPAG_LootManagerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OFPLANETSANDGUNS_API UOPAG_LootManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UOPAG_LootManagerComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot Manager")
		UDataTable* DataTable;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SpawnLoot(const float DifficultyValue) const;
};
