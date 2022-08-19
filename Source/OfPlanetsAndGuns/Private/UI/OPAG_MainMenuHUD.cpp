// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OPAG_MainMenuHUD.h"
#include "Managers/OPAG_EventManagerSubsystem.h"
#include "Managers/OPAG_GameInstance.h"
#include "UI/OPAG_UWMainMenuView.h"
#include "UI/OPAG_UWHubView.h"
#include "UI/OPAG_UWUpgradeSystemView.h"

void AOPAG_MainMenuHUD::DrawHUD()
{
	Super::DrawHUD();
}

void AOPAG_MainMenuHUD::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PC = GetWorld()->GetFirstPlayerController();

	if (PC) {
		PC->SetInputMode(FInputModeUIOnly());
		PC->bShowMouseCursor = true;
	}

	if (MainMenuViewWidgetClass)
	{
		MainMenuViewWidget = CreateWidget<UOPAG_UWMainMenuView>(GetWorld(), MainMenuViewWidgetClass);
		
		if (MainMenuViewWidget) {
			MainMenuViewWidget->AddToViewport(1);
		}
	}

	if (HubViewWidgetClass)
	{
		HubViewWidget = CreateWidget<UOPAG_UWHubView>(GetWorld(), HubViewWidgetClass);
	}

	if (UpgradeSystemWidgetClass)
	{
		UpgradeSystemWidget = CreateWidget<UOPAG_UWUpgradeSystemView>(GetWorld(), UpgradeSystemWidgetClass);
	}
}

void AOPAG_MainMenuHUD::ShowHubView()
{
	if (MainMenuViewWidget) {
		MainMenuViewWidget->RemoveFromViewport();
	}

	if (HubViewWidget) {
		HubViewWidget->AddToViewport(1);
	}
}

void AOPAG_MainMenuHUD::ShowUpgradeSystemView()
{
	if (HubViewWidget) {
		HubViewWidget->RemoveFromViewport();
	}

	if (UpgradeSystemWidget) {
		UpgradeSystemWidget->AddToViewport(1);
	}
}