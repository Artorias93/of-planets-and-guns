#include "UI/OPAG_UWCreditsView.h"

#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "UI/OPAG_MainMenuHUD.h"

void UOPAG_UWCreditsView::NativeConstruct()
{
	Super::NativeConstruct();

	CurrentPage = 0;

	PagesList.Add(Pag0);
	PagesList.Add(Pag1);
	PagesList.Add(Pag2);

	//Collapsing every guide page except for the first one
	for(int i = 0; i < PagesList.Num(); i++)
	{
		if(i == CurrentPage)
		{
			PagesList[i]->SetVisibility(ESlateVisibility::Visible);
			continue;
		}
		PagesList[i]->SetVisibility(ESlateVisibility::Collapsed);
	}

	//Collapsing the prev button (because on the first page)
	PreviousButton->SetVisibility(ESlateVisibility::Collapsed);

	//Binding delegates
	PreviousButton->OnClicked.AddUniqueDynamic(this, &UOPAG_UWCreditsView::PreviousButtonClicked);
	NextButton->OnClicked.AddUniqueDynamic(this, &UOPAG_UWCreditsView::NextButtonClicked);
	BackButton->OnClicked.AddUniqueDynamic(this, &UOPAG_UWCreditsView::BackButtonClicked);
}

void UOPAG_UWCreditsView::PreviousButtonClicked()
{
	if(CurrentPage <= 0)
		return;

	CurrentPage--;

	if(CurrentPage == 0)
	{
		PreviousButton->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		PreviousButton->SetVisibility(ESlateVisibility::Visible);
	}
			
	if(CurrentPage == PagesList.Num() - 1)
	{
		NextButton->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{		
		NextButton->SetVisibility(ESlateVisibility::Visible);
	}
	
	for(int i = 0; i < PagesList.Num(); i++)
	{
		if(i == CurrentPage)
		{
			PagesList[i]->SetVisibility(ESlateVisibility::Visible);
			continue;
		}
		PagesList[i]->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UOPAG_UWCreditsView::NextButtonClicked()
{
	if(CurrentPage >= PagesList.Num() - 1)
	 	return;

	CurrentPage++;

	if(CurrentPage == 0)
	{
		PreviousButton->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		PreviousButton->SetVisibility(ESlateVisibility::Visible);
	}
			
	if(CurrentPage == PagesList.Num() - 1)
	{
		NextButton->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{		
		NextButton->SetVisibility(ESlateVisibility::Visible);
	}
		
	for(int i = 0; i < PagesList.Num(); i++)
	{
		if(i == CurrentPage)
		{
			PagesList[i]->SetVisibility(ESlateVisibility::Visible);
			continue;
		}
		PagesList[i]->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UOPAG_UWCreditsView::BackButtonClicked()
{
	if(const AOPAG_MainMenuHUD* const MyHUD = Cast<AOPAG_MainMenuHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
	{
		MyHUD->SequencePlaySelection(Credits, MainMenu);
	}
}