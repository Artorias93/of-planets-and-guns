// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OPAG_UWMainMenuView.generated.h"

/**
 * 
 */
class UButton;

UCLASS()
class OFPLANETSANDGUNS_API UOPAG_UWMainMenuView : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
		UButton* ContinueRunButton;
	UPROPERTY(meta = (BindWidget))
		UButton* NewRunButton;
	UPROPERTY(meta = (BindWidget))
		UButton* OptionsButton;
	UPROPERTY(meta = (BindWidget))
		UButton* QuitButton;
};
