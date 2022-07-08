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
		class UOPAG_UWPlayerView* PlayerViewWidget;
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<UOPAG_UWPlayerView> PlayerViewWidgetClass;

public:
	virtual void DrawHUD() override;

};
