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
class UHorizontalBox;
class UTexture2D;

UCLASS()
class OFPLANETSANDGUNS_API UOPAG_UWPlayerStatsView : public UUserWidget
{
	GENERATED_BODY()

	void BindEvents();

	UPROPERTY(meta = (BindWidget))
		UImage* DashIconOld;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* DashAmountText;
	UPROPERTY(meta = (BindWidget))
		UProgressBar* DashChargeBar;

	UPROPERTY(meta =(BindWidget))
		UHorizontalBox* DashBox;
	UPROPERTY(meta = (BindWidget))
		UProgressBar* HealthBar;
	UPROPERTY(meta = (BindWidget))
		UProgressBar* ShieldBar;

public:
	UFUNCTION()
	void UpdateHealthBar(const float CurrentValue, const float MaxValue);
	UFUNCTION()
	void UpdateShieldBar(const float CurrentValue, const float MaxValue);
	UFUNCTION()
	void UpdateDashAmountText(const int CurrentValue);
	UFUNCTION()
	void UpdateDashChargeBar(const float CurrentValue, const float MaxValue);
	UFUNCTION()
	void IncrementDashAmount();
	UFUNCTION()
	void DecrementDashAmount();

	void NativeConstruct();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		UTexture2D* DashIconTexture;
};
