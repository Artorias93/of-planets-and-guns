// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OPAG_UWCurrencyView.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Managers/OPAG_GameInstance.h"
#include "Managers/OPAG_EventManagerSubsystem.h"
#include "Blueprint/WidgetTree.h"

void UOPAG_UWCurrencyView::NativeConstruct() {
	BindEvents();
}

void UOPAG_UWCurrencyView::BindEvents()
{
	UOPAG_EventManagerSubsystem* EMS = GetGameInstance<UOPAG_GameInstance>()->GetEventManager();

	if (EMS) {
		EMS->BoltsAmountChanged.AddUniqueDynamic(this, &UOPAG_UWCurrencyView::UpdateBoltsAmount);
		EMS->PotionsAmountChanged.AddUniqueDynamic(this, &UOPAG_UWCurrencyView::UpdatePotionsAmount);
		EMS->GoldChanged.AddUniqueDynamic(this, &UOPAG_UWCurrencyView::UpdateGoldValue);
		EMS->ShowGoldInGame.AddUniqueDynamic(this, &UOPAG_UWCurrencyView::ShowGold);
		EMS->HideGoldInGame.AddUniqueDynamic(this, &UOPAG_UWCurrencyView::HideGold);
		EMS->UpdateAccessoryInfo.AddUniqueDynamic(this, &UOPAG_UWCurrencyView::UpdateAccessoryIcon);
	}

	GoldIcon->SetVisibility(ESlateVisibility::Hidden);
	GoldText->SetVisibility(ESlateVisibility::Hidden);
}

void UOPAG_UWCurrencyView::UpdateBoltsAmount(const int CurrentValue)
{
	if (BoltsText && CurrentValue >= 0) {
		BoltsText->SetText(FText::FromString(FString::FromInt(CurrentValue)));
	}
}

void UOPAG_UWCurrencyView::UpdatePotionsAmount(const int CurrentValue)
{
	if (PotionsText && CurrentValue >= 0) {
		PotionsText->SetText(FText::FromString(FString::FromInt(CurrentValue)));
	}
}

void UOPAG_UWCurrencyView::UpdateGoldValue(const int Value)
{
	if (GoldText && Value >= 0)
	{
		GoldText->SetText(FText::FromString(FString::FromInt(Value)));
	}
}

void UOPAG_UWCurrencyView::ShowGold()
{
	if (GoldText && GoldIcon)
	{
		GoldIcon->SetVisibility(ESlateVisibility::Visible);
		GoldText->SetVisibility(ESlateVisibility::Visible);
	}
}

void UOPAG_UWCurrencyView::HideGold()
{
	if (GoldText && GoldIcon)
	{
		GoldIcon->SetVisibility(ESlateVisibility::Hidden);
		GoldText->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UOPAG_UWCurrencyView::UpdateAccessoryIcon(const EAccessoryType Value)
{
	if (BoltsIcon) 
	{
		BoltsIcon->SetBrushFromTexture(AccessoryTextures2D[(int)Value]);
	}
}