// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OPAG_UWPlayerView.generated.h"

/**
 * 
 */
class UTextBlock;
class UProgressBar;
class UImage;

UCLASS()
class OFPLANETSANDGUNS_API UOPAG_UWPlayerView : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
		UImage* GoldIcon;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* GoldAmountText;
	UPROPERTY(meta = (BindWidget))
		UImage* BoltsIcon;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* BoltsAmountText;

	UPROPERTY(meta = (BindWidget))
		UImage* DashIcons;

	UPROPERTY(meta = (BindWidget))
		UProgressBar* HealthBar;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* HealthAmountText;
	UPROPERTY(meta = (BindWidget))
		UProgressBar* ShieldBar; 
	UPROPERTY(meta = (BindWidget))
		UTextBlock* ShieldAmountText;

	UPROPERTY(meta = (BindWidget))
		UImage* EquippedGunIcon;
	UPROPERTY(meta = (BindWidget))
		UImage* PreviousGunIcon;
	UPROPERTY(meta = (BindWidget))
		UImage* NextGunIcon;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* CurrentAmmoText;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* TotalAmmoText;
		
};
