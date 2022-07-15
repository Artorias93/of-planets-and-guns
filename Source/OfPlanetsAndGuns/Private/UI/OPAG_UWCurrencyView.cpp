// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OPAG_UWCurrencyView.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Managers/OPAG_GameInstance.h"
#include "Managers/OPAG_EventManagerSubsystem.h"

void UOPAG_UWCurrencyView::NativeConstruct() {
	BindEvents();
}

void UOPAG_UWCurrencyView::BindEvents()
{
	UOPAG_EventManagerSubsystem* EMS = GetGameInstance<UOPAG_GameInstance>()->GetEventManager();

	if (EMS) {
		EMS->GoldChanged.AddDynamic(this, &UOPAG_UWCurrencyView::UpdateGoldValue);
		EMS->BoltsChanged.AddDynamic(this, &UOPAG_UWCurrencyView::UpdateBoltsValue);
	}
}

void UOPAG_UWCurrencyView::UpdateBoltsValue(const int Value)
{
	if (BoltsAmountText && Value >= 0) {
		BoltsAmountText->SetText(FText::FromString(FString::FromInt(Value)));
	}
}

void UOPAG_UWCurrencyView::UpdateGoldValue(const int Value)
{
	if (GoldAmountText && Value >= 0) {
		GoldAmountText->SetText(FText::FromString(FString::FromInt(Value)));
	}
}
