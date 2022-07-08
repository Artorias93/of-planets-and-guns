// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OPAG_HubHUD.h"
#include "UI/OPAG_UWHubView.h"

void AOPAG_HubHUD::DrawHUD()
{
	Super::DrawHUD();
}

void AOPAG_HubHUD::BeginPlay()
{
	Super::BeginPlay();

	if (HubViewWidgetClass)
	{
		HubViewWidget = CreateWidget<UOPAG_UWHubView>(GetWorld(), HubViewWidgetClass);

		if (HubViewWidget) {
			HubViewWidget->AddToViewport(1);
		}
	}
}