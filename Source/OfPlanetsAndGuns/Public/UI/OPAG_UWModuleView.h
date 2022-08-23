// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Weapons/Guns/OPAG_GunStats.h"
#include "Weapons/Modules/OPAG_ModuleStats.h"
#include "OPAG_UWModuleView.generated.h"

class UImage;
class UTextBlock;
/**
 * 
 */
UCLASS()
class OFPLANETSANDGUNS_API UOPAG_UWModuleView : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
		UImage* ModuleBackground;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* ModuleName;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* ModuleRarity;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* ModuleType;

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
		void UpdateModuleValues(const FOPAG_GunStats& GunStats, const FOPAG_GunStats& CurrentModuleStats, 
			const FOPAG_GunStats& OtherModuleStats, const FString OtherModuleName, const EModuleRarity OtherModuleRarity, const EModuleTypes OtherModuleType);

	void NativeConstruct();

	void UpdateSingleStat(UTextBlock* StatText, UTextBlock* StatAmount, UTextBlock* DeltaText, float BaseAmount, float OldValue, 
		float NewValue, bool bIsInverted = false);

	void UpdateTextColor(UTextBlock* StatText, UTextBlock* DeltaText, FLinearColor Color);
};