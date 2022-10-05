#include "UI/OPAG_UWGunCompareView.h"
#include "Weapons/Guns/OPAG_GunBase.h"
#include "Kismet/KismetTextLibrary.h"
#include "Weapons/Modules/OPAG_ModuleBase.h"
#include "Components/TextBlock.h"

void UOPAG_UWGunCompareView::NativeConstruct()
{
	Super::NativeConstruct();
}

void UOPAG_UWGunCompareView::UpdateGunValues(const FOPAG_GunStats& GunStats, const FOPAG_GunStats& OtherGunStats, 
	const FString OtherGunName, const FString OtherGunFireType)
{
	GunName->SetText(FText::FromString(OtherGunName));
	GunType->SetText(FText::FromString(OtherGunFireType));
	UpdateSingleStat(DamageText, DamageAmount, DamageDelta, GunStats.Damage, OtherGunStats.Damage);
	UpdateSingleStat(FireRateText, FireRateAmount, FireRateDelta, GunStats.FireRate, OtherGunStats.FireRate);
	UpdateSingleStat(AccuracyText, AccuracyAmount, AccuracyDelta, GunStats.Accuracy, OtherGunStats.Accuracy);
	UpdateSingleStat(StabilityText, StabilityAmount, StabilityDelta, GunStats.Stability, OtherGunStats.Stability);
	UpdateSingleStat(ReloadingText, ReloadingAmount, ReloadingDelta, GunStats.ReloadingTime, OtherGunStats.ReloadingTime, true);
	UpdateSingleStat(ClipSizeText, ClipSizeAmount, ClipSizeDelta, GunStats.ClipSize, OtherGunStats.ClipSize);
}

void UOPAG_UWGunCompareView::UpdateSingleStat(UTextBlock* StatText, UTextBlock* StatAmount, UTextBlock* DeltaText, float OldValue, float NewValue,
	bool bIsInverted)
{
	const float Delta = NewValue - OldValue;

	FString DeltaString;

	if (NewValue > OldValue) {
		if (bIsInverted)
			UpdateTextColor(StatText, DeltaText, RedColor);
		else
			UpdateTextColor(StatText, DeltaText, GreenColor);

		DeltaString.AppendChar('+');
	}
	else if (NewValue == OldValue) {
		UpdateTextColor(StatText, DeltaText, WhiteColor);
	}
	else if (NewValue < OldValue) {
		if (bIsInverted)
			UpdateTextColor(StatText, DeltaText, GreenColor);
		else
			UpdateTextColor(StatText, DeltaText, RedColor);
	}

	StatAmount->SetText(UKismetTextLibrary::Conv_FloatToText(NewValue, ERoundingMode::FromZero, false, true, 1, 324, 2, 2));

	DeltaString.Append(FString::Printf(TEXT("%.2f"), Delta));

	DeltaText->SetText(FText::FromString(DeltaString));
}

void UOPAG_UWGunCompareView::UpdateTextColor(UTextBlock* StatText, UTextBlock* DeltaText, FLinearColor Color)
{
	StatText->SetColorAndOpacity(Color);
	DeltaText->SetColorAndOpacity(Color);
}
