// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OPAG_GameplayHUD.h"
#include "UI/OPAG_UWGunView.h"
#include "UI/OPAG_UWCurrencyView.h"
#include "UI/OPAG_UWPlayerStatsView.h"
#include "UI/OPAG_UWPauseView.h"

void AOPAG_GameplayHUD::DrawHUD() 
{
	Super::DrawHUD();
}

void AOPAG_GameplayHUD::BeginPlay()
{
	Super::BeginPlay();

	if (PlayerStatsViewWidgetClass)
	{
		PlayerStatsViewWidget = CreateWidget<UOPAG_UWPlayerStatsView>(GetWorld(), PlayerStatsViewWidgetClass);

		if (PlayerStatsViewWidget) {
			PlayerStatsViewWidget->AddToViewport(1);
		}
	}

	if (GunViewWidgetClass)
	{
		GunViewWidget = CreateWidget<UOPAG_UWGunView>(GetWorld(), GunViewWidgetClass);

		if (GunViewWidget) {
			GunViewWidget->AddToViewport(1);
		}
	}

	if (CurrencyViewWidgetClass)
	{
		CurrencyViewWidget = CreateWidget<UOPAG_UWCurrencyView>(GetWorld(), CurrencyViewWidgetClass);

		if (CurrencyViewWidget) {
			CurrencyViewWidget->AddToViewport(1);
		}
	}

	if (PauseViewWidgetClass)
	{
		PauseViewWidget = CreateWidget<UOPAG_UWPauseView>(GetWorld(), PauseViewWidgetClass);
	}

	//Reference to EventManager?
	//EventManager->PauseGame.AddDynamic(this, &AOPAG_GameplayHUD::ShowPauseView);
}

void AOPAG_GameplayHUD::ShowPauseView() {

	//Cache a FirstPlayerController reference?
	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeUIOnly());
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;

	if (PauseViewWidget) {
		PauseViewWidget->AddToViewport(2);
	}
}

void AOPAG_GameplayHUD::HidePauseView() {

	//Cache a FirstPlayerController reference?
	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;

	if (PauseViewWidget) {
		PauseViewWidget->RemoveFromViewport();
	}

	//Event Dispatch->ResumeGame Event! (to Event Manager?)
}