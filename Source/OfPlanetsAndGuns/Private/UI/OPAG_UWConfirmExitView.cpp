#include "UI/OPAG_UWConfirmExitView.h"
#include "Components/Button.h"
#include "UI/OPAG_MainMenuHUD.h"

void UOPAG_UWConfirmExitView::NativeConstruct() {
	Super::NativeConstruct();
	BindEvents();
}

void UOPAG_UWConfirmExitView::BindEvents()
{
	ExitYesButton->OnClicked.AddDynamic(this, &UOPAG_UWConfirmExitView::ExitYesButtonClicked);
	ExitNoButton->OnClicked.AddDynamic(this, &UOPAG_UWConfirmExitView::ExitNoButtonClicked);
}

void UOPAG_UWConfirmExitView::ExitYesButtonClicked()
{
	if (AOPAG_MainMenuHUD* const MyHUD = Cast<AOPAG_MainMenuHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
	{
		MyHUD->HideExitMenu();
		ExitYesButton->OnClicked.RemoveDynamic(this, &UOPAG_UWConfirmExitView::ExitYesButtonClicked);
		MyHUD->SequencePlaySelection(Exit, QuitGame);		
	}
}

void UOPAG_UWConfirmExitView::ExitNoButtonClicked()
{
	if (AOPAG_MainMenuHUD* const MyHUD = Cast<AOPAG_MainMenuHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
	{
		MyHUD->HideExitMenu();
		ExitNoButton->OnClicked.RemoveDynamic(this, &UOPAG_UWConfirmExitView::ExitNoButtonClicked);
		MyHUD->SequencePlaySelection(Exit, MainMenu);
	}
}