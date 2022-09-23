// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OPAG_UWOptionsView.generated.h"

/**
 * 
 */
class USlider;
class UButton;

UCLASS()
class OFPLANETSANDGUNS_API UOPAG_UWOptionsView : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
		USlider* MasterSlider;
	UPROPERTY(meta = (BindWidget))
		USlider* MusicSlider;
	UPROPERTY(meta = (BindWidget))
		USlider* SFXSlider;
	UPROPERTY(meta = (BindWidget))
		USlider* AmbientSlider;
	UPROPERTY(meta = (BindWidget))
		USlider* SystemSlider;

	UPROPERTY(meta = (BindWidget))
		USlider* GraphicsSlider;

	UPROPERTY(meta = (BindWidget))
		UButton* BackButton;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		FLinearColor RedColor;

public:
	void NativeConstruct();

	UFUNCTION()
		void BackButtonClicked();
	UFUNCTION()
		void GraphicsSliderChanged(float CurrentValue);
};
