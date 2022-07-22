
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "OPAG_RoomOffset.generated.h"
/**
 *
 */

UENUM(BlueprintType, Meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class ERoomExits : uint8
{
	None = 0x00,
	Left = 0x01,
	Right = 0x02
};
ENUM_CLASS_FLAGS(ERoomExits);


USTRUCT(BlueprintType)
struct OFPLANETSANDGUNS_API FOPAG_RoomOffset : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UWorld> Room;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Weight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 ExitCount;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bitmask, BitmaskEnum = ERoomExits))
		uint8 ExitSides;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FTransform Offset;

};