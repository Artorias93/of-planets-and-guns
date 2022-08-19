// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "OPAG_HubHUD.generated.h"

class UOPAG_UWUpgradeSystemView;

UCLASS()
class OFPLANETSANDGUNS_API AOPAG_HubHUD : public AHUD
{
	GENERATED_BODY()

	void BindEvents();

private:
	UPROPERTY()
		class UOPAG_UWUpgradeSystemView* UpgradeSystemWidget;
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<UOPAG_UWUpgradeSystemView> UpgradeSystemViewWidgetClass;

protected:
	virtual void BeginPlay() override;

public:
	virtual void DrawHUD() override;

	UFUNCTION()
		void ShowUpgradeSystem();
	
};
