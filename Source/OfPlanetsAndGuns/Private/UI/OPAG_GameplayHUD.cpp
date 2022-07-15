// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OPAG_GameplayHUD.h"
#include "UI/OPAG_UWGunView.h"
#include "UI/OPAG_UWCurrencyView.h"
#include "UI/OPAG_UWPlayerStatsView.h"
#include "UI/OPAG_UWPauseView.h"
#include "UI/OPAG_UWLoadingScreenView.h"
#include "Managers/OPAG_GameInstance.h"
#include "Managers/OPAG_EventManagerSubsystem.h"


void AOPAG_GameplayHUD::DrawHUD() 
{
	Super::DrawHUD();
}

void AOPAG_GameplayHUD::BindEvents() {
	UOPAG_EventManagerSubsystem* EMS = GetGameInstance<UOPAG_GameInstance>()->GetEventManager();
	EMS->PauseGame.AddDynamic(this, &AOPAG_GameplayHUD::ShowPauseView);
	EMS->StageLoaded.AddDynamic(this, &AOPAG_GameplayHUD::HideLoadingScreenView);
}

void AOPAG_GameplayHUD::BeginPlay()
{
	Super::BeginPlay();

	BindEvents();

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

	if (LoadingScreenViewWidgetClass)
	{
		LoadingScreenViewWidget = CreateWidget<UOPAG_UWLoadingScreenView>(GetWorld(), LoadingScreenViewWidgetClass); 
		
		if (LoadingScreenViewWidget) {
			LoadingScreenViewWidget->AddToViewport(3);
		}
	}

	if (PauseViewWidgetClass)
	{
		PauseViewWidget = CreateWidget<UOPAG_UWPauseView>(GetWorld(), PauseViewWidgetClass);
	}
}

void AOPAG_GameplayHUD::ShowPauseView() {

	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeUIOnly());
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;

	if (PauseViewWidget) {
		PauseViewWidget->AddToViewport(2);
	}
}

void AOPAG_GameplayHUD::HidePauseView() {

	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;

	if (PauseViewWidget) {
		PauseViewWidget->RemoveFromViewport();
	}

	UOPAG_EventManagerSubsystem* EMS = GetGameInstance<UOPAG_GameInstance>()->GetEventManager();
	EMS->ResumeGame.Broadcast();
}

void AOPAG_GameplayHUD::ShowLoadingScreenView()
{
	
}

void AOPAG_GameplayHUD::HideLoadingScreenView()
{
	if (LoadingScreenViewWidget) {
		LoadingScreenViewWidget->RemoveFromViewport();
	}
}
