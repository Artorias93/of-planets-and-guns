// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "OPAG_DoubleExitChance.generated.h"

USTRUCT(BlueprintType)
struct OFPLANETSANDGUNS_API FOPAG_DoubleExitChance : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(UIMin=1, UIMax=100))
		float Chance = 1;
	/** How many double exit rooms need to be already loaded for this chance to be active */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int MinCountForChance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int GuaranteedSpawnRoomIndex = -1;
};