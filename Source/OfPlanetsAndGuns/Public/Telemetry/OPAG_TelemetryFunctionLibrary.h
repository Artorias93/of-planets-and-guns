// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "OPAG_TelemetryFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class OFPLANETSANDGUNS_API UOPAG_TelemetryFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	static FProperty* RetrieveProperty(AActor* InActor, const FString& InPath, void*& OutTarget);
};
