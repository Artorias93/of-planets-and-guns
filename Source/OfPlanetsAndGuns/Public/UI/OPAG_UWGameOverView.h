// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OPAG_UWGameOverView.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class OFPLANETSANDGUNS_API UOPAG_UWGameOverView : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
		UButton* ExitButton;

public:

	UFUNCTION()
		void ExitButtonClicked();

	virtual void NativeConstruct() override;
};
