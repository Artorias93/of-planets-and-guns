// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "OPAG_StageGeneration.generated.h"

USTRUCT(BlueprintType)
struct FOPAG_StageGeneration : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stage)
		int32 StageIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pathing)
		int32 TargetWeight;
	/** How many rooms out it will keep geometry loaded in */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Streaming")
		int32	FarthestLoad;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Data")
		int32	MaxDoubleExitRooms;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Data")
		UDataTable* RoomTable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Data")
		UDataTable* BridgeTable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Data")
		UDataTable* DoubleRoomChanceTable;

	FORCEINLINE bool operator<(const FOPAG_StageGeneration& Other) const
	{
		return StageIndex < Other.StageIndex;
	}
};


