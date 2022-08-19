// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OPAG_UWMainMenuView.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Managers/OPAG_GameInstance.h"
#include "Managers/OPAG_EventManagerSubsystem.h"
#include "UI/OPAG_MainMenuHUD.h"
#include "OPAG_MainMenuGameMode.h"
#include "Kismet/GameplayStatics.h" 

void UOPAG_UWMainMenuView::NativeConstruct() {
	Super::NativeConstruct();
	BindEvents();
}

void UOPAG_UWMainMenuView::BindEvents()
{
	NewRunButton->OnClicked.AddDynamic(this, &UOPAG_UWMainMenuView::NewRunButtonClicked);
	OptionsButton->OnClicked.AddDynamic(this, &UOPAG_UWMainMenuView::OptionsButtonClicked);
	QuitButton->OnClicked.AddDynamic(this, &UOPAG_UWMainMenuView::QuitButtonClicked);
}

void UOPAG_UWMainMenuView::NewRunButtonClicked()
{
	NewRunButton->OnClicked.RemoveDynamic(this, &UOPAG_UWMainMenuView::NewRunButtonClicked);

	AOPAG_MainMenuHUD* MyHUD = Cast<AOPAG_MainMenuHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (MyHUD)
		MyHUD->ShowHubView();
}

void UOPAG_UWMainMenuView::OptionsButtonClicked()
{

}

void UOPAG_UWMainMenuView::QuitButtonClicked()
{

}