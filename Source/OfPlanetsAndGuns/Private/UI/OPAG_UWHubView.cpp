// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OPAG_UWHubView.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Managers/OPAG_GameInstance.h"
#include "Managers/OPAG_EventManagerSubsystem.h"


void UOPAG_UWHubView::NativeConstruct()
{
	Super::NativeConstruct();
	BindEvents();
}

void UOPAG_UWHubView::BindEvents()
{

}

/*
void UOPAG_UWHubView::UpdateModuleName(UTextBlock* TextBlock, const FString& Name)
{
	if(TextBlock)
		TextBlock->SetText(FText::FromString(Name));
}
*/

void UOPAG_UWHubView::UpdateBoltsValue(int Value)
{
	/*
	if (BoltsAmountText && Value >= 0) {
		BoltsAmountText->SetText(FText::FromString(FString::FromInt(Value)));
	}
	*/
}

void UOPAG_UWHubView::UpdatePotionsValue(int Value)
{
	/*
	if (PotionsAmountText && Value >= 0) {
		PotionsAmountText->SetText(FText::FromString(FString::FromInt(Value)));
	}
	*/
}
