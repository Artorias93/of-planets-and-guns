// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OPAG_MainMenuHUD.h"
#include "UI/OPAG_UWMainMenuView.h"

void AOPAG_MainMenuHUD::DrawHUD()
{
	Super::DrawHUD();
}

void AOPAG_MainMenuHUD::BeginPlay()
{
	Super::BeginPlay();

	if (MainMenuViewWidgetClass)
	{
		MainMenuViewWidget = CreateWidget<UOPAG_UWMainMenuView>(GetWorld(), MainMenuViewWidgetClass);

		if (MainMenuViewWidget) {
			MainMenuViewWidget->AddToViewport(1);
		}
	}
}