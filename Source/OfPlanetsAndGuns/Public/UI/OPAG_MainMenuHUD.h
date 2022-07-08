// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "OPAG_MainMenuHUD.generated.h"

/**
 * 
 */
UCLASS()
class OFPLANETSANDGUNS_API AOPAG_MainMenuHUD : public AHUD
{
	
	GENERATED_BODY()
	
	virtual void BeginPlay() override;

private:
	UPROPERTY()
		class UOPAG_UWMainMenuView* MainMenuViewWidget;
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<UOPAG_UWMainMenuView> MainMenuViewWidgetClass;

public:
	virtual void DrawHUD() override;
	
};
