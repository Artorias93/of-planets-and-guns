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
		/*
		EMS->PotionsAmountIncrement.AddDynamic(this, &UOPAG_UWCurrencyView::IncrementPotionsAmount);
		EMS->PotionsAmountDecrement.AddDynamic(this, &UOPAG_UWCurrencyView::DecrementPotionsAmount);

		EMS->BoltsAmountIncrement.AddDynamic(this, &UOPAG_UWCurrencyView::IncrementBoltsAmount);
		EMS->BoltsAmountDecrement.AddDynamic(this, &UOPAG_UWCurrencyView::DecrementBoltsAmount);
		*/

		EMS->BoltsAmountChanged.AddDynamic(this, &UOPAG_UWCurrencyView::UpdateBoltsAmount);
		EMS->PotionsAmountChanged.AddDynamic(this, &UOPAG_UWCurrencyView::UpdatePotionsAmount);
	}
}

void UOPAG_UWCurrencyView::IncrementPotionsAmount()
{
	UImage* const NewPotionsIcon = WidgetTree->ConstructWidget<UImage>();

	if (PotionsIconTexture) {
		NewPotionsIcon->SetBrushFromTexture(PotionsIconTexture);
	}

	if (NewPotionsIcon)
		PotionsBox->AddChild(NewPotionsIcon);

	UHorizontalBoxSlot* const MyBoxSlot = CastChecked<UHorizontalBoxSlot>(PotionsBox->GetSlots()[(PotionsBox->GetChildrenCount() - 1)]);

	if (MyBoxSlot) {
		MyBoxSlot->SetPadding(FMargin(5, 5, 5, 5));
	}
}

void UOPAG_UWCurrencyView::DecrementPotionsAmount()
{
	if (PotionsBox->GetChildrenCount() > 0) {
		PotionsBox->RemoveChildAt((PotionsBox->GetChildrenCount() - 1));
	}
}

void UOPAG_UWCurrencyView::IncrementBoltsAmount()
{
	UImage* const NewBoltsIcon = WidgetTree->ConstructWidget<UImage>();

	if (BoltsIconTexture) {
		NewBoltsIcon->SetBrushFromTexture(BoltsIconTexture);
	}

	if (NewBoltsIcon)
		BoltsBox->AddChild(NewBoltsIcon);

	UHorizontalBoxSlot* const MyBoxSlot = CastChecked<UHorizontalBoxSlot>(BoltsBox->GetSlots()[(BoltsBox->GetChildrenCount() - 1)]);

	if (MyBoxSlot) {
		MyBoxSlot->SetPadding(FMargin(5, 5, 5, 5));
	}
}

void UOPAG_UWCurrencyView::DecrementBoltsAmount()
{
	if (BoltsBox->GetChildrenCount() > 0) {
		BoltsBox->RemoveChildAt((BoltsBox->GetChildrenCount() - 1));
	}
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