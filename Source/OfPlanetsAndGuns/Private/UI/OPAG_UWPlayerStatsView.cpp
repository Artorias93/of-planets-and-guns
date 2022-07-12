// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OPAG_UWPlayerStatsView.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

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
