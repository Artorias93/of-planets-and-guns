// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OPAG_UWLoadingScreenView.generated.h"

/**
 * 
 */

class UTextBlock;
class UProgressBar;
class UImage;

UCLASS()
class OFPLANETSANDGUNS_API UOPAG_UWLoadingScreenView : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
		UImage* LoadingBackgroundImage;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* LoadingText;
	UPROPERTY(meta = (BindWidget))
		UProgressBar* LoadingBar;

public:
	void SetLoadingBar(float Percentage);
};
