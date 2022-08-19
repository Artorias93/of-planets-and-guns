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

	UPROPERTY(meta = (BindWidget))
		UButton* PlayButton;

	UPROPERTY(meta = (BindWidget))
		UButton* UpgradeSystemButton;

	UPROPERTY(meta = (BindWidget))
		UButton* PreviousPlanetButton;
	UPROPERTY(meta = (BindWidget))
		UButton* NextPlanetButton;

	void BindEvents();

public:
	void NativeConstruct();

	UFUNCTION()
		void UpgradeSystemButtonClicked();
};
