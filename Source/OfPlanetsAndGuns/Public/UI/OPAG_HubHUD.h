// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "OPAG_HubHUD.generated.h"

/**
 * 
 */
UCLASS()
class OFPLANETSANDGUNS_API AOPAG_HubHUD : public AHUD
{
	GENERATED_BODY()

	virtual void BeginPlay() override;

private:
	UPROPERTY()
		class UOPAG_UWHubView* HubViewWidget;
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<UOPAG_UWHubView> HubViewWidgetClass;

public:
	virtual void DrawHUD() override;
};
