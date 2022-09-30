// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "OPAG_ModuleStatItem.generated.h"

enum EDeltaSign
{
	Negative,
	Positive,
	Zero
};

/**
 * 
 */
UCLASS()
class OFPLANETSANDGUNS_API UOPAG_ModuleStatItem : public UObject
{
	GENERATED_BODY()
	
public:
	
	UOPAG_ModuleStatItem();

	void Init(FString _Name, FString _Value, FString _Delta, EDeltaSign _eDeltaSign, bool _bIsInverted);

	FString Name;
	FString Value;
	FString Delta;

	// bool bIsNegative;
	EDeltaSign DeltaSign;
	bool bIsInverted;
};
