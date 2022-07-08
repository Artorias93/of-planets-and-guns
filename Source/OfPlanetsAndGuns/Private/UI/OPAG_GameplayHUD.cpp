// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OPAG_GameplayHUD.h"
#include "UI/OPAG_UWPlayerView.h"

void AOPAG_GameplayHUD::DrawHUD() 
{
	Super::DrawHUD();
}

void AOPAG_GameplayHUD::BeginPlay()
{
	Super::BeginPlay();

	if (PlayerViewWidgetClass)
	{
		PlayerViewWidget = CreateWidget<UOPAG_UWPlayerView>(GetWorld(), PlayerViewWidgetClass);

		if (PlayerViewWidget) {
			PlayerViewWidget->AddToViewport(1);
		}
	}
}
