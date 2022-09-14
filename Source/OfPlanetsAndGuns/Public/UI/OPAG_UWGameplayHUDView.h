// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OPAG_UWGameplayHUDView.generated.h"

/**
 * 
 */
class URetainerBox;

UCLASS()
class OFPLANETSANDGUNS_API UOPAG_UWGameplayHUDView : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	URetainerBox* GameplayRetainerBox;
};
