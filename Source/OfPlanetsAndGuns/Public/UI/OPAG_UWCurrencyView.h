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

	UPROPERTY(meta = (BindWidget))
		UImage* PotionsIcon;
	UPROPERTY(meta = (BindWidget))
		UImage* BoltsIcon;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* PotionsText;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* BoltsText;

public:
	//Unused
	UFUNCTION()
		void IncrementPotionsAmount();
	UFUNCTION()
		void DecrementPotionsAmount();
	//Unused
	UFUNCTION()
		void IncrementBoltsAmount();
	UFUNCTION()
		void DecrementBoltsAmount();
	//

	UFUNCTION()
		void UpdateBoltsAmount(const int CurrentValue);
	UFUNCTION()
		void UpdatePotionsAmount(const int CurrentValue);

	void NativeConstruct();

	//Unused
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		UTexture2D* PotionsIconTexture;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		UTexture2D* BoltsIconTexture;
	//
};
