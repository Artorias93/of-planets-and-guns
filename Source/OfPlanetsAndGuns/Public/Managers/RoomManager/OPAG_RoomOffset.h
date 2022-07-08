
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "OPAG_RoomOffset.generated.h"
/**
 *
 */

USTRUCT(BlueprintType)
struct FOPAG_RoomOffset : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName RoomName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Weight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 ExitCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bitmask, BitmaskEnum = ERoomExit))
		int32 ExitSides;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FTransform Offset;

};