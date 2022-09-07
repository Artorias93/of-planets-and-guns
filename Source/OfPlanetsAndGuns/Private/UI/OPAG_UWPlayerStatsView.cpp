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

void UOPAG_UWPlayerStatsView::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UOPAG_UWPlayerStatsView::BindEvents()
{
	UOPAG_EventManagerSubsystem* EMS = GetGameInstance<UOPAG_GameInstance>()->GetEventManager();
	
	if (EMS) {
		EMS->HealthChanged.AddDynamic(this, &UOPAG_UWPlayerStatsView::UpdateHealthBar);
		EMS->ShieldChanged.AddDynamic(this, &UOPAG_UWPlayerStatsView::UpdateShieldBar);

		EMS->DashInit.AddDynamic(this, &UOPAG_UWPlayerStatsView::DashInit);
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

void UOPAG_UWPlayerStatsView::UpdateShieldBar(const float CurrentValue, const float MaxValue)
{
	float Percentage = CurrentValue / MaxValue;

	if (ShieldBar && (Percentage >= 0.0f && Percentage <= 1.0f)) {
		ShieldBar->SetPercent(Percentage);
	}
}

void UOPAG_UWPlayerStatsView::DashInit()
{
	UProgressBar* const NewDashProgressBar = WidgetTree->ConstructWidget<UProgressBar>();

	if (NewDashProgressBar)
	{
		NewDashProgressBar->SetPercent(1.f);
		NewDashProgressBar->SetFillColorAndOpacity(FLinearColor::Red);
		DashBox->AddChild(NewDashProgressBar);
	}

	UHorizontalBoxSlot* const MyBoxSlot = CastChecked<UHorizontalBoxSlot>(DashBox->GetSlots()[(DashBox->GetChildrenCount() - 1)]);

	if (MyBoxSlot) {
		MyBoxSlot->SetPadding(FMargin(5, 5, 5, 5));
	}

	Dashes.Add(NewDashProgressBar);
	NextToBeUsedDashBoxSlot = Dashes.Num() - 1;
}

void UOPAG_UWPlayerStatsView::IncrementDashAmount()
{
	Dashes[NextToBeUsedDashBoxSlot]->SetPercent(1.0f);

	if (NextToBeUsedDashBoxSlot < 5)
		NextToBeUsedDashBoxSlot++;

	UProgressBar* const LastUsedDashProgBar = Dashes[NextToBeUsedDashBoxSlot];

	if (LastUsedDashProgBar)
		LastRechargingDashBoxSlot = LastUsedDashProgBar;
}

void UOPAG_UWPlayerStatsView::DecrementDashAmount()
{
	UProgressBar* const LastUsedDashProgBar = Dashes[NextToBeUsedDashBoxSlot];

	if (LastUsedDashProgBar) {
		LastUsedDashProgBar->SetPercent(0.f);

		if (NextToBeUsedDashBoxSlot > 0)
			NextToBeUsedDashBoxSlot--;
	}
}
