// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "OPAG_GameplayHUD.generated.h"

/**
 * 
 */

enum class EModuleRarity : uint8;
struct FOPAG_GunStats;

UCLASS()
class OFPLANETSANDGUNS_API AOPAG_GameplayHUD : public AHUD
{
	GENERATED_BODY()

	void BindEvents();

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
	UPROPERTY()
		class UOPAG_UWLoadingScreenView* LoadingScreenViewWidget;
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<UOPAG_UWLoadingScreenView> LoadingScreenViewWidgetClass;
	UPROPERTY()
		class UOPAG_UWModuleView* ModuleViewWidget;
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<UOPAG_UWModuleView> ModuleViewWidgetClass;
	

public:
	virtual void DrawHUD() override;

	UFUNCTION()
	void ShowPauseView();
	UFUNCTION()
	void HidePauseView();

	UFUNCTION()
	void UpdateLoadingScreenBar(float Percentage);
	UFUNCTION()
	void ShowLoadingScreenView();
	UFUNCTION()
	void HideLoadingScreenView();

	UFUNCTION()
	void ShowModuleCompareView(const FOPAG_GunStats GunStats, const FOPAG_GunStats CurrentModule,
		const FOPAG_GunStats OtherModule, const FString OtherModuleName, const EModuleRarity OtherModuleRarity);
	UFUNCTION()
	void HideModuleCompareView();
};