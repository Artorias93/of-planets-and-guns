#include "UI/OPAG_UWConfirmExitView.h"
#include "Components/Button.h"
#include "UI/OPAG_MainMenuHUD.h"

void UOPAG_UWConfirmExitView::NativeConstruct() {
	Super::NativeConstruct();
	BindEvents();
}

void UOPAG_UWConfirmExitView::BindEvents()
{
	ExitYesButton->OnClicked.AddUniqueDynamic(this, &UOPAG_UWConfirmExitView::ExitYesButtonClicked);
	ExitNoButton->OnClicked.AddUniqueDynamic(this, &UOPAG_UWConfirmExitView::ExitNoButtonClicked);
}

void UOPAG_UWConfirmExitView::ExitYesButtonClicked()
{
	if (const AOPAG_MainMenuHUD* const MyHUD = Cast<AOPAG_MainMenuHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
	{
		MyHUD->SequencePlaySelection(Exit, QuitGame);
	}
}

void UOPAG_UWConfirmExitView::ExitNoButtonClicked()
{
	if (const AOPAG_MainMenuHUD* const MyHUD = Cast<AOPAG_MainMenuHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
	{
		MyHUD->SequencePlaySelection(Exit, MainMenu);
	}
}