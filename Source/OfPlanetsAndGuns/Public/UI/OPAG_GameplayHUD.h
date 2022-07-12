// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "OPAG_GameplayHUD.generated.h"

/**
 * 
 */
UCLASS()
class OFPLANETSANDGUNS_API AOPAG_GameplayHUD : public AHUD
{
	GENERATED_BODY()
	
	virtual void BeginPlay() override;

private:
	UPROPERTY()
		class UOPAG_UWPlayerStatsView* PlayerStatsViewWidget;
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<UOPAG_UWPlayerStatsView> PlayerStatsViewWidgetClass;
	UPROPERTY()
		class UOPAG_UWGunView* GunViewWidget;
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<UOPAG_UWGunView> GunViewWidgetClass;
	UPROPERTY()
		class UOPAG_UWCurrencyView* CurrencyViewWidget;
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<UOPAG_UWCurrencyView> CurrencyViewWidgetClass;
	UPROPERTY()
		class UOPAG_UWPauseView* PauseViewWidget;
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<UOPAG_UWPauseView> PauseViewWidgetClass;

public:
	virtual void DrawHUD() override;
};
