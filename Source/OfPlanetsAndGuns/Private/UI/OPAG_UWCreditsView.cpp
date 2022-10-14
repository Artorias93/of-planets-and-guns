#include "UI/OPAG_UWCreditsView.h"

#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "UI/OPAG_MainMenuHUD.h"

void UOPAG_UWCreditsView::NativeConstruct()
{
	Super::NativeConstruct();

	BackButton->OnClicked.AddUniqueDynamic(this, &UOPAG_UWCreditsView::BackButtonClicked);
}


void UOPAG_UWCreditsView::BackButtonClicked()
{
	if(const AOPAG_MainMenuHUD* const MyHUD = Cast<AOPAG_MainMenuHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
	{
		MyHUD->SequencePlaySelection(Credits, MainMenu);
	}
}