// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelSequencePlayer.h"
#include "GameFramework/HUD.h"
#include "OPAG_MainMenuHUD.generated.h"

/**
 * 
 */
UCLASS()
class OFPLANETSANDGUNS_API AOPAG_MainMenuHUD : public AHUD
{
	GENERATED_BODY()

	void BindEvents();

	UFUNCTION()
	void ShowMainMenu();
	
private:
	UPROPERTY()
		class UOPAG_UWMainMenuView* MainMenuViewWidget;
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<UOPAG_UWMainMenuView> MainMenuViewWidgetClass;
	UPROPERTY()
		class UOPAG_UWHubView* HubViewWidget;
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<UOPAG_UWHubView> HubViewWidgetClass;

protected:
	virtual void BeginPlay() override;

public:
	virtual void DrawHUD() override;

	UFUNCTION()
	void ShowHubView();
};
