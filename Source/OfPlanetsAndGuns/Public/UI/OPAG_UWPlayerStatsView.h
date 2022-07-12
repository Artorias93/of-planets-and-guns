// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OPAG_UWPlayerStatsView.generated.h"

/**
 * 
 */
class UTextBlock;
class UProgressBar;
class UImage;

UCLASS()
class OFPLANETSANDGUNS_API UOPAG_UWPlayerStatsView : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
		UImage* DashIcons;

	UPROPERTY(meta = (BindWidget))
		UProgressBar* HealthBar;
	//UPROPERTY(meta = (BindWidget))
		//UTextBlock* HealthAmountText;
	UPROPERTY(meta = (BindWidget))
		UProgressBar* ShieldBar;
	//UPROPERTY(meta = (BindWidget))
		//UTextBlock* ShieldAmountText;
};
