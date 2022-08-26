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
};
