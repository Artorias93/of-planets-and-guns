// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Weapons/Guns/OPAG_GunStats.h"
#include "Weapons/Modules/OPAG_ModuleStats.h"
#include "OPAG_UWGunCompareView.generated.h"

/**
 * 
 */
class UImage;
class UTextBlock;

UCLASS()
class OFPLANETSANDGUNS_API UOPAG_UWGunCompareView : public UUserWidget
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
		UTextBlock* RecoilText;
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
		UTextBlock* RecoilAmount;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* ReloadingAmount;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* ClipSizeAmount;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* DamageDelta;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* FireRateDelta;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* AccuracyDelta;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* RecoilDelta;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* ReloadingDelta;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* ClipSizeDelta;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		FLinearColor RedColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		FLinearColor GreenColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		FLinearColor WhiteColor;

public:
	UFUNCTION()
	void UpdateGunValues(const FOPAG_GunStats& GunStats, const FOPAG_GunStats& OtherGunStats, const FString OtherGunName, const EFireTypes OtherGunType);

	void NativeConstruct();

	void UpdateSingleStat(UTextBlock* StatText, UTextBlock* StatAmount, UTextBlock* DeltaText, float OldValue, float NewValue, bool bIsInverted = false);
	
	void UpdateTextColor(UTextBlock* StatText, UTextBlock* DeltaText, FLinearColor Color);
};
