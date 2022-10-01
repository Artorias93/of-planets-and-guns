// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "OPAG_ModuleStatVertItem.generated.h"

/**
 * 
 */
UCLASS()
class OFPLANETSANDGUNS_API UOPAG_ModuleStatVertItem : public UObject
{
	GENERATED_BODY()

public:

	UOPAG_ModuleStatVertItem();

	void Init(FString _DamageText, FString _FireRateText, FString _AccuracyText,
		FString _StabilityText, FString _ReloadingText, FString _ClipSizeText);

	FString DamageValue;
	FString FireRateValue;
	FString AccuracyValue;
	FString StabilityValue;
	FString ReloadingValue;
	FString ClipSizeValue;

};