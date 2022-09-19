#include "UI/OPAG_UWHowToPlayView.h"

#include "Components/Button.h"
#include "UI/OPAG_MainMenuHUD.h"

void UOPAG_UWHowToPlayView::NativeConstruct()
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

	//Binding delegates
	PreviousButton->OnClicked.AddUniqueDynamic(this, &UOPAG_UWHowToPlayView::PreviousButtonClicked);
	NextButton->OnClicked.AddUniqueDynamic(this, &UOPAG_UWHowToPlayView::NextButtonClicked);
	BackButton->OnClicked.AddUniqueDynamic(this, &UOPAG_UWHowToPlayView::BackButtonClicked);
}

void UOPAG_UWHowToPlayView::PreviousButtonClicked()
{
	if(CurrentPage <= 0)
		return;

	CurrentPage--;
	
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

void UOPAG_UWHowToPlayView::NextButtonClicked()
{
	if(CurrentPage >= PagesList.Num() - 1)
	 	return;

	CurrentPage++;
		
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

void UOPAG_UWHowToPlayView::BackButtonClicked()
{
	if(const AOPAG_MainMenuHUD* const MyHUD = Cast<AOPAG_MainMenuHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
	{
		MyHUD->SequencePlaySelection(HowToPlay, MainMenu);
	}
}