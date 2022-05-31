// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "FLevelOffset.generated.h"
/**
 *
 */

USTRUCT(BlueprintType)
struct FLevelOffset : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName LevelName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Exits;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FTransform Offset;

};