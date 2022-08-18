// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OPAG_UpgradeSystemHUD.h"
#include "Managers/OPAG_EventManagerSubsystem.h"
#include "Managers/OPAG_GameInstance.h"
#include "UI/OPAG_UWUpgradeSystemView.h"
#include "UI/OPAG_UWHubView.h"


void AOPAG_UpgradeSystemHUD::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PC = GetWorld()->GetFirstPlayerController();

	if (PC) 
	{
		PC->SetInputMode(FInputModeUIOnly());
		PC->bShowMouseCursor = true;
	}

	if (UpgradeSystemViewWidgetClass)
	{
		UpgradeSystemWidget = CreateWidget<UOPAG_UWUpgradeSystemView>(GetWorld(), UpgradeSystemViewWidgetClass);

		if (UpgradeSystemWidget) {
			UpgradeSystemWidget->AddToViewport(1);
		}
	}
}

void AOPAG_UpgradeSystemHUD::DrawHUD()
{
	Super::DrawHUD();
}

void AOPAG_UpgradeSystemHUD::ShowUpgradeSystem()
{
	if (UpgradeSystemWidget) 
	{
		UpgradeSystemWidget->AddToViewport(1);
	}

	const UOPAG_EventManagerSubsystem* const EMS = GetGameInstance<UOPAG_GameInstance>()->GetEventManager();
}
