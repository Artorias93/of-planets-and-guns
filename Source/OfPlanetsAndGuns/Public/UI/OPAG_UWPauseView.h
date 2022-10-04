// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OPAG_UWPauseView.generated.h"

/**
 * 
 */

class UTextBlock;
class UImage;
class UButton;
class UListView;

class AOPAG_ModuleBase;
struct FOPAG_GunStats;

UCLASS()
class OFPLANETSANDGUNS_API UOPAG_UWPauseView : public UUserWidget
{
	GENERATED_BODY()

	void BindEvents();

	UPROPERTY(meta = (BindWidget))
		UImage* GoldIcon;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* GoldAmountText;


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

	UPROPERTY(meta = (BindWidget))
		UListView* StatsListView;

	UPROPERTY(meta = (BindWidget))
		UImage* PauseBackgroundImage;
	UPROPERTY(meta = (BindWidget))
		UButton* ContinueButton;
	UPROPERTY(meta = (BindWidget))
		UButton* OptionsButton;
	UPROPERTY(meta = (BindWidget))
		UButton* ExitButton;

public:
	UFUNCTION()
	void ContinueButtonClicked();
	UFUNCTION()
	void OptionsButtonClicked(); 
	UFUNCTION()
	void ExitButtonClicked();

	UFUNCTION()
	void UpdateGoldValue(const int Value);

	void NativeConstruct();

	void UpdateGunValues(const FOPAG_GunStats GunStats,
		const FString GunName, const FString GunFireType, TArray<AOPAG_ModuleBase*> Modules);
};
