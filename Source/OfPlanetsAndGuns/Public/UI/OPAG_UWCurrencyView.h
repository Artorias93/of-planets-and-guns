// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OPAG_UWCurrencyView.generated.h"

/**
 * 
 */
class UTextBlock;
class UImage;

UCLASS()
class OFPLANETSANDGUNS_API UOPAG_UWCurrencyView : public UUserWidget
{
	GENERATED_BODY()

	void BindEvents();

	UPROPERTY(meta = (BindWidget))
		UImage* GoldIcon;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* GoldAmountText;
	UPROPERTY(meta = (BindWidget))
		UImage* BoltsIcon;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* BoltsAmountText;
	UPROPERTY(meta = (BindWidget))
		UImage* PotionsIcon;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* PotionsAmountText;

public:
	UFUNCTION()
		void UpdateBoltsValue(const int Value);
	UFUNCTION()
		void UpdateGoldValue(const int Value);
	UFUNCTION()
		void UpdatePotionsValue(const int Value);

	void NativeConstruct();
};
