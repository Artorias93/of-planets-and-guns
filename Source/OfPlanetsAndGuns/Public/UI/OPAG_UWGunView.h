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

	UPROPERTY(meta = (BindWidget))
		UImage* EquippedGunIcon;
	UPROPERTY(meta = (BindWidget))
		UImage* PreviousGunIcon;
	UPROPERTY(meta = (BindWidget))
		UImage* NextGunIcon;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* CurrentAmmoText;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* TotalAmmoText;
};
