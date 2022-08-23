// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OPAG_UWHubView.generated.h"

/**
 * 
 */
class UTextBlock;
class UButton;

UCLASS()
class OFPLANETSANDGUNS_API UOPAG_UWHubView : public UUserWidget
{
	GENERATED_BODY()

	/*
		//Weapon's Modules
	UPROPERTY(meta = (BindWidget))
		UTextBlock* BarrelName;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* StockName;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* GripName;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* MagazineName;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* SightName;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* AccessoryName;
	

	//Player's stuff
	UPROPERTY(meta = (BindWidget))
		UTextBlock* BoltsAmountText;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* PotionsAmountText;
	*/
	UPROPERTY(meta = (BindWidget))
		UButton* PlayButton;

	void BindEvents();

public:

	void NativeConstruct();
	/*
	UFUNCTION()
		void UpdateModuleName(UTextBlock* TextBlock, const FString& Name);
	*/
	UFUNCTION()
		void UpdateBoltsValue (int Value);
	UFUNCTION()
		void UpdatePotionsValue(int Value);
};
