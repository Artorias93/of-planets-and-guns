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

	UE_LOG(LogTemp, Warning, TEXT("1"));

	if (PlayerViewWidgetClass)
	{
		PlayerViewWidget = CreateWidget<UOPAG_UWPlayerView>(GetWorld(), PlayerViewWidgetClass);

		UE_LOG(LogTemp, Warning, TEXT("2"));

		if (PlayerViewWidget) {
			UE_LOG(LogTemp, Warning, TEXT("3"));
			PlayerViewWidget->AddToViewport(1);
		}
	}
}
