#include "UI/OPAG_UWLevelSelectionView.h"
#include "Components/Button.h"
#include "UI/OPAG_MainMenuHUD.h"
#include "UI/OPAG_UWMainMenuView.h"

void UOPAG_UWLevelSelectionView::NativeConstruct()
{
	Super::NativeConstruct();
	
	Planet1Button->OnClicked.AddUniqueDynamic(this, &UOPAG_UWLevelSelectionView::Planet1ButtonClicked);
	BackButton->OnClicked.AddUniqueDynamic(this, &UOPAG_UWLevelSelectionView::BackButtonClicked);
}

void UOPAG_UWLevelSelectionView::Planet1ButtonClicked()
{
	if(const AOPAG_MainMenuHUD* const MyHUD = Cast<AOPAG_MainMenuHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
	{
		MyHUD->SequencePlaySelection(LevelSelection, Level1);
	}	
}

void UOPAG_UWLevelSelectionView::BackButtonClicked()
{
	if(const AOPAG_MainMenuHUD* const MyHUD = Cast<AOPAG_MainMenuHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
	{
		MyHUD->SequencePlaySelection(LevelSelection, MainMenu);
	}	
}
