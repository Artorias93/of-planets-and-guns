// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Managers/RoomManager/OPAG_StreamingRoom.h"
//#include "OPAG_StagePathing.generated.h"

//USTRUCT(BlueprintType)
struct FOPAG_StagePathing// : public FTableRowBase
{
	//GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FOPAG_StreamingRoom* CurrentRoom;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 PrevRoomID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<int32> NextRoomIDs;

	~FOPAG_StagePathing() { delete CurrentRoom; };
};