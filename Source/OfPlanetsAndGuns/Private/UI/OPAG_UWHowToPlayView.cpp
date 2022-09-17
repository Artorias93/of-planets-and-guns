#include "UI/OPAG_UWHowToPlayView.h"

void UOPAG_UWHowToPlayView::NativeConstruct()
{
	Super::NativeConstruct();
	PreviousButton->OnClicked.AddUniqueDynamic(this, &UOPAG_UWHowToPlayView::PreviousButtonClicked);
	NextButton->OnClicked.AddUniqueDynamic(this, &UOPAG_UWHowToPlayView::NextButtonClicked);
	BackButton->OnClicked.AddUniqueDynamic(this, &UOPAG_UWHowToPlayView::BackButtonClicked);
}

void UOPAG_UWHowToPlayView::PreviousButtonClicked()
{
	
}

void UOPAG_UWHowToPlayView::NextButtonClicked()
{
	
}

void UOPAG_UWHowToPlayView::BackButtonClicked()
{
	
}