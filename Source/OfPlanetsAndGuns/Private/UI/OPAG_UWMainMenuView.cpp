#include "UI/OPAG_UWMainMenuView.h"
#include "Components/Image.h"
#include "Components/Button.h"
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
	HowToPlayButton->OnClicked.AddUniqueDynamic(this, &UOPAG_UWMainMenuView::HowToPlayButtonClicked);
	CreditsButton->OnClicked.AddUniqueDynamic(this, &UOPAG_UWMainMenuView::CreditsButtonClicked);
}

void UOPAG_UWMainMenuView::NewRunButtonClicked()
{
	if (const AOPAG_MainMenuHUD* const MyHUD = Cast<AOPAG_MainMenuHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
	{
		MyHUD->SequencePlaySelection(MainMenu, LevelSelection);
	}
}

void UOPAG_UWMainMenuView::OptionsButtonClicked()
{
	if (const AOPAG_MainMenuHUD* const MyHUD = Cast<AOPAG_MainMenuHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
	{
		MyHUD->SequencePlaySelection(MainMenu, Options);
	}	
}

void UOPAG_UWMainMenuView::QuitButtonClicked()
{
	if (const AOPAG_MainMenuHUD* const MyHUD = Cast<AOPAG_MainMenuHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
	{
		MyHUD->SequencePlaySelection(MainMenu, Exit);
	}	
}

void UOPAG_UWMainMenuView::HowToPlayButtonClicked()
{
	if (const AOPAG_MainMenuHUD* const MyHUD = Cast<AOPAG_MainMenuHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
	{
		MyHUD->SequencePlaySelection(MainMenu, HowToPlay);
	}	
}

void UOPAG_UWMainMenuView::CreditsButtonClicked()
{
	if (const AOPAG_MainMenuHUD* const MyHUD = Cast<AOPAG_MainMenuHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
	{
		MyHUD->SequencePlaySelection(MainMenu, Credits);
	}	
}
