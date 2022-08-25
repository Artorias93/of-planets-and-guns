// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OPAG_UWInteractView.generated.h"

/**
 * 
 */
class UTextBlock;

UCLASS()
class OFPLANETSANDGUNS_API UOPAG_UWInteractView : public UUserWidget
{
	GENERATED_BODY()

	void BindEvents();

	UPROPERTY(meta = (BindWidget))
	UTextBlock* InteractText;
	
	void NativeConstruct();

public:
	UFUNCTION()
	void UpdateInteractText(bool bCanInteract, const FString InteractTextParam);
};
