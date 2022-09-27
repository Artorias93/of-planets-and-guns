// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Weapons/Guns/OPAG_GunStats.h"
#include "OPAG_UWPauseMenuGunView.generated.h"

/**
 * 
 */

class UImage;
class UTextBlock;

UCLASS()
class OFPLANETSANDGUNS_API UOPAG_UWPauseMenuGunView : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
		UImage* GunBackground;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* GunName;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* GunType;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* DamageText;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* FireRateText;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* AccuracyText;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* StabilityText;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* ReloadingText;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* ClipSizeText;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* DamageAmount;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* FireRateAmount;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* AccuracyAmount;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* StabilityAmount;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* ReloadingAmount;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* ClipSizeAmount;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		FLinearColor RedColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		FLinearColor GreenColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		FLinearColor WhiteColor;

public:
	UFUNCTION()
	void UpdateGunValues(const FOPAG_GunStats GunStats, const FString MyGunName, const FString GunFireType);

	void NativeConstruct();

	void UpdateSingleStat(UTextBlock* StatText, UTextBlock* StatAmount, float Value);
	
	void BindEvents();
};
