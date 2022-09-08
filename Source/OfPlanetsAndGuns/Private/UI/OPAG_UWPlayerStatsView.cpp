// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OPAG_UWPlayerStatsView.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Managers/OPAG_GameInstance.h"
#include "Managers/OPAG_EventManagerSubsystem.h"
#include "Blueprint/WidgetTree.h"

void UOPAG_UWPlayerStatsView::NativeConstruct() {
	Super::NativeConstruct();
	BindEvents();
}

void UOPAG_UWPlayerStatsView::BindEvents()
{
	UOPAG_EventManagerSubsystem* EMS = GetGameInstance<UOPAG_GameInstance>()->GetEventManager();
	
	if (EMS) {
		EMS->HealthChanged.AddDynamic(this, &UOPAG_UWPlayerStatsView::UpdateHealthBar);
		EMS->ShieldChanged.AddDynamic(this, &UOPAG_UWPlayerStatsView::UpdateShieldBar);

		EMS->DashAmountIncrement.AddDynamic(this, &UOPAG_UWPlayerStatsView::IncrementDashAmount);
		EMS->DashAmountDecrement.AddDynamic(this, &UOPAG_UWPlayerStatsView::DecrementDashAmount);
	}
}

void UOPAG_UWPlayerStatsView::UpdateHealthBar(const float CurrentValue, const float MaxValue)
{
	float Percentage = CurrentValue / MaxValue;

	if (HealthBar && (Percentage >= 0.0f && Percentage <= 1.0f)) {
		HealthBar->SetPercent(Percentage);
	}
}

void UOPAG_UWPlayerStatsView::UpdateShieldBar(const float CurrentValue, const float MaxValue, const bool IsBroken)
{
	float Percentage = CurrentValue / MaxValue;

	if (IsBroken)
	{
		ShieldBar->SetFillColorAndOpacity(BrokenShieldColor);
	}
	else
	{
		ShieldBar->SetFillColorAndOpacity(ActiveShieldColor);
	}

	if (ShieldBar && (Percentage >= 0.0f && Percentage <= 1.0f)) {
		ShieldBar->SetPercent(Percentage);
	}
}

void UOPAG_UWPlayerStatsView::IncrementDashAmount()
{
	UImage* const NewDashIcon = WidgetTree->ConstructWidget<UImage>();

	if (DashIconTexture) {
		NewDashIcon->SetBrushFromTexture(DashIconTexture);
	}

	if (NewDashIcon)
		DashBox->AddChild(NewDashIcon);

	UHorizontalBoxSlot* const MyBoxSlot = CastChecked<UHorizontalBoxSlot>(DashBox->GetSlots()[(DashBox->GetChildrenCount() - 1)]);

	if (MyBoxSlot) {
		MyBoxSlot->SetPadding(FMargin(5, 5, 5, 5));
	}
}

void UOPAG_UWPlayerStatsView::DecrementDashAmount()
{
	if (DashBox->GetChildrenCount() > 0) {
		DashBox->RemoveChildAt((DashBox->GetChildrenCount() - 1));
	}
}
