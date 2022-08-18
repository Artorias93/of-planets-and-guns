// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OPAG_UWUpgradeSystemItemView.generated.h"

class UImage;
class UTextBlock;

UCLASS()
class OFPLANETSANDGUNS_API UOPAG_UWUpgradeSystemItemView : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
		UImage* AbilityLevel1;
	UPROPERTY(meta = (BindWidget))
		UImage* AbilityLevel2;
	UPROPERTY(meta = (BindWidget))
		UImage* AbilityLevel3;
	UPROPERTY(meta = (BindWidget))
		UImage* AbilityLevel4;
	UPROPERTY(meta = (BindWidget))
		UImage* AbilityLevel5;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* AbilityName;
	
};
