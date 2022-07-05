// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FLevelOffset.h"
#include "LevelStreamingFunctionLibrary.generated.h"

UCLASS()
class OFPLANETSANDGUNS_API ULevelStreamingFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable)
	static bool AddToDataTable(UPARAM(Ref) FLevelOffset& levelOffsetRow, UDataTable* dataTable);
};
