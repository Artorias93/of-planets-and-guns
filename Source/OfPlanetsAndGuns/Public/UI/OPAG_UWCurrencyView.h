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
class UHorizontalBox;

UCLASS()
class OFPLANETSANDGUNS_API UOPAG_UWCurrencyView : public UUserWidget
{
	GENERATED_BODY()

	void BindEvents();

	UPROPERTY(meta = (BindWidget))
		UHorizontalBox* PotionsBox;
	UPROPERTY(meta = (BindWidget))
		UHorizontalBox* BoltsBox;

public:
	UFUNCTION()
		void IncrementPotionsAmount();
	UFUNCTION()
		void DecrementPotionsAmount();

	UFUNCTION()
		void IncrementBoltsAmount();
	UFUNCTION()
		void DecrementBoltsAmount();

	void NativeConstruct();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		UTexture2D* PotionsIconTexture;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		UTexture2D* BoltsIconTexture;
};
