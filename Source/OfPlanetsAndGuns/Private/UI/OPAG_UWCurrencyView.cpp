// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OPAG_UWCurrencyView.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UOPAG_UWCurrencyView::UpdateBoltsValue(const int Value)
{
	if (BoltsAmountText && Value >= 0) {
		BoltsAmountText->SetText(FText::FromString(FString::FromInt(Value)));
		//Trigger sound and some kind of animation on the icon
	}
}

void UOPAG_UWCurrencyView::UpdateGoldValue(const int Value)
{
	if (GoldAmountText && Value >= 0) {
		GoldAmountText->SetText(FText::FromString(FString::FromInt(Value)));
		//Trigger sound and some kind of animation on the icon
	}
}
