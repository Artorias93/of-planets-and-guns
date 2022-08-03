// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OPAG_UWModuleView.h"
#include "Weapons/Guns/OPAG_GunBase.h"
#include "Components/TextBlock.h"

void UOPAG_UWModuleView::NativeConstruct()
{
	Super::NativeConstruct();
}

void UOPAG_UWModuleView::UpdateModuleValues(const FOPAG_GunStats& GunStats, const FOPAG_GunStats& CurrentModuleStats,
	const FOPAG_GunStats& OtherModuleStats, const FString OtherModuleName, const EModuleRarity OtherModuleRarity)
{
	ModuleName->SetText(FText::FromString(OtherModuleName));
	ModuleRarity->SetText(FText::FromString("No Method - Do it"));

	UpdateSingleStat(DamageText, DamageAmount, DamageDelta, GunStats.Damage, CurrentModuleStats.Damage, OtherModuleStats.Damage);
	UpdateSingleStat(FireRateText, FireRateAmount, FireRateDelta, GunStats.FireRate, CurrentModuleStats.FireRate, OtherModuleStats.FireRate);
	UpdateSingleStat(AccuracyText, AccuracyAmount, AccuracyDelta, GunStats.Accuracy, CurrentModuleStats.Accuracy, OtherModuleStats.Accuracy);
	UpdateSingleStat(RecoilText, RecoilAmount, RecoilDelta, GunStats.RecoilRate, CurrentModuleStats.RecoilRate, OtherModuleStats.RecoilRate);
	UpdateSingleStat(ReloadingText, ReloadingAmount, ReloadingDelta, GunStats.ReloadingTime, CurrentModuleStats.ReloadingTime, OtherModuleStats.ReloadingTime);
	UpdateSingleStat(ClipSizeText, ClipSizeAmount, ClipSizeDelta, GunStats.ClipSize, CurrentModuleStats.ClipSize, OtherModuleStats.ClipSize);
}

void UOPAG_UWModuleView::UpdateSingleStat(UTextBlock* StatText, UTextBlock* StatAmount, UTextBlock* DeltaText, float BaseAmount, float OldValue, float NewValue) {
	float NewStat = BaseAmount + NewValue;
	float OldStat = BaseAmount + OldValue;
	float Delta = NewStat - OldStat;

	FString DeltaString;

	if (NewStat > OldStat) {
		StatText->SetColorAndOpacity(FSlateColor(FLinearColor(FVector(0,1,0))));
		DeltaText->SetColorAndOpacity(FSlateColor(FLinearColor(FVector(0,1,0))));

		DeltaString.AppendChar('+');
	}
	else if (NewStat == OldStat) {
		StatText->SetColorAndOpacity(FSlateColor(FLinearColor(FVector(1, 1, 1))));
		DeltaText->SetColorAndOpacity(FSlateColor(FLinearColor(FVector(1, 1, 1))));
	}
	else if (NewStat < OldStat) {
		StatText->SetColorAndOpacity(FSlateColor(FLinearColor(FVector(1, 0, 0))));
		DeltaText->SetColorAndOpacity(FSlateColor(FLinearColor(FVector(1, 0, 0))));
	}

	StatAmount->SetText(FText::FromString(FString::SanitizeFloat(NewStat)));

	DeltaString.Append(FString::SanitizeFloat(Delta));

	DeltaText->SetText(FText::FromString(DeltaString));
}
