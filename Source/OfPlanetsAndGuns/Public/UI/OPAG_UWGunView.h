// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OPAG_UWGunView.generated.h"

/**
 * 
 */
class UTextBlock;

class UImage;

UCLASS()
class OFPLANETSANDGUNS_API UOPAG_UWGunView : public UUserWidget
{
	GENERATED_BODY()

	void BindEvents();

	UPROPERTY(meta = (BindWidget))
		UImage* GunIcon;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* CurrentAmmoText;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* TotalAmmoText;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* AmmoSeparatorText;

public:
	UFUNCTION()
		void UpdateCurrentAmmoText(const int CurrentValue);
	UFUNCTION()
		void UpdateTotalAmmoText(const int CurrentValue); 
	UFUNCTION()
		void UpdateAmmoColor(const FLinearColor ColorParam);

	void NativeConstruct();
};
