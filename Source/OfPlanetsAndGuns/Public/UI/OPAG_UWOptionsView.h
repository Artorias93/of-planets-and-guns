// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OPAG_UWInputMappingView.h"
#include "Blueprint/UserWidget.h"
#include "Managers/OPAG_Settings.h"
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
	
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UOPAG_UWInputMappingView> MappingViewWidgetClass;
	UPROPERTY()
	UOPAG_UWInputMappingView* MappingViewWidget;

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
		USlider* AimSensSlider;
	UPROPERTY(meta = (BindWidget))
		USlider* AdsAimSensSlider;

	UPROPERTY(meta = (BindWidget))
		UButton* BackButton;
	UPROPERTY(meta = (BindWidget))
	UButton* MappingButton;

	class UOPAG_EventManagerSubsystem* EMS;

	class UOPAG_SoundManagerSubsystem* SMS;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		FLinearColor RedColor;

public:
	void NativeConstruct();

	UFUNCTION()
		void LoadSettings(const FOPAG_Settings& Settings);
	UFUNCTION()
		void BackButtonClicked();
	UFUNCTION()
		void MappingButtonClicked();
	UFUNCTION()
		void GraphicsSliderChanged(float CurrentValue);

	UFUNCTION()
		void MasterSliderChanged(float CurrentValue);
	UFUNCTION()
		void MusicSliderChanged(float CurrentValue);
	UFUNCTION()
		void SFXSliderChanged(float CurrentValue);
	UFUNCTION()
		void AmbientSliderChanged(float CurrentValue);
	UFUNCTION()
		void SystemSliderChanged(float CurrentValue);
};
