// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemies/OPAG_Enemy.h"
#include "Engine/DataTable.h"
#include "OPAG_EnemyLevel.generated.h"


/**
 * 
 */

USTRUCT(BlueprintType)
struct OFPLANETSANDGUNS_API FOPAG_EnemyLevel : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AOPAG_Enemy> Enemy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Level;

};
