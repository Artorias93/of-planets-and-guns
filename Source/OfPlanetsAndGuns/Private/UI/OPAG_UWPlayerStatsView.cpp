#include "UI/OPAG_UWPlayerStatsView.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Managers/OPAG_GameInstance.h"
#include "Managers/OPAG_EventManagerSubsystem.h"
#include "Blueprint/WidgetTree.h"
#include "Characters/Player/OPAG_Player.h"
#include "Characters/Player/OPAG_AttributeSetPlayer.h"

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
		EMS->UpdateDashAmount.AddDynamic(this, &UOPAG_UWPlayerStatsView::UpdateDashAmount);
		EMS->UpdateDashProgressBar.AddDynamic(this, &UOPAG_UWPlayerStatsView::UpdateDashProgressCooldown);
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

void UOPAG_UWPlayerStatsView::UpdateDashAmount(float CurrentValue)
{
	CurrentDashCount->SetText(FText::FromString(FString::FromInt(FMath::RoundToInt(CurrentValue))));
}

void UOPAG_UWPlayerStatsView::UpdateDashProgressCooldown(float TimeProgress)
{
	DashProgressBar->SetPercent(TimeProgress);

	if(DashProgressBar->Percent >= 1.f)	DashProgressBar->SetPercent(.0f);
}