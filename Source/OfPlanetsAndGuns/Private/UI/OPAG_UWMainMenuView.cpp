// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OPAG_UWMainMenuView.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Managers/OPAG_GameInstance.h"
#include "Managers/OPAG_EventManagerSubsystem.h"
#include "UI/OPAG_MainMenuHUD.h"

void UOPAG_UWMainMenuView::NativeConstruct() {
	Super::NativeConstruct();
	BindEvents();
}

void UOPAG_UWMainMenuView::BindEvents()
{
	NewRunButton->OnClicked.AddUniqueDynamic(this, &UOPAG_UWMainMenuView::NewRunButtonClicked);
	OptionsButton->OnClicked.AddUniqueDynamic(this, &UOPAG_UWMainMenuView::OptionsButtonClicked);
	QuitButton->OnClicked.AddUniqueDynamic(this, &UOPAG_UWMainMenuView::QuitButtonClicked);
}

void UOPAG_UWMainMenuView::NewRunButtonClicked()
{
	if (AOPAG_MainMenuHUD* const MyHUD = Cast<AOPAG_MainMenuHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
		MyHUD->ShowHubView();
}

void UOPAG_UWMainMenuView::OptionsButtonClicked()
{
	if (AOPAG_MainMenuHUD* const MyHUD = Cast<AOPAG_MainMenuHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
	{
		//MyHUD->HideMainMenu();
		MyHUD->SequencePlaySelection(MainMenu, Options);
	}	
}

void UOPAG_UWMainMenuView::QuitButtonClicked()
{
	if (AOPAG_MainMenuHUD* const MyHUD = Cast<AOPAG_MainMenuHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
	{
		//MyHUD->HideMainMenu();
		MyHUD->SequencePlaySelection(MainMenu, Exit);
	}	

}

