// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OPAG_RoomOffset.h"
#include "Engine/DataTable.h"
#include "Engine/LevelStreamingDynamic.h"
#include "OPAG_StreamingRoom.generated.h"

USTRUCT(BlueprintType)
struct FOPAG_StreamingRoom : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 ID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ULevelStreamingDynamic* LevelStreaming;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FOPAG_RoomOffset RoomOffset;
};
