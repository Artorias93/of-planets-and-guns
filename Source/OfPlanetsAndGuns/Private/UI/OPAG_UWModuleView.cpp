// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OPAG_UWModuleView.h"
#include "Weapons/Guns/OPAG_GunBase.h"
#include "Kismet/KismetTextLibrary.h"
#include "Weapons/Modules/OPAG_ModuleBase.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UOPAG_UWModuleView::NativeConstruct()
{
	Super::NativeConstruct();
}

void UOPAG_UWModuleView::UpdateModuleValues(const FOPAG_GunStats& GunStats, const FOPAG_GunStats& CurrentModuleStats,
	const FOPAG_GunStats& OtherModuleStats, const FString OtherModuleName, const EModuleRarity OtherModuleRarity, const EModuleTypes OtherModuleType, 
	const int EquippableTypes)
{
	FullyAutomaticIcon->SetBrushTintColor(FLinearColor::White);
	SingleShotIcon->SetBrushTintColor(FLinearColor::White);
	Burst3Icon->SetBrushTintColor(FLinearColor::White);
	ShotgunIcon->SetBrushTintColor(FLinearColor::White);

	ModuleName->SetText(FText::FromString(OtherModuleName));
	ModuleRarity->SetText(FText::FromString(AOPAG_ModuleBase::GetRarityName(OtherModuleRarity)));
	ModuleType->SetText(FText::FromString(AOPAG_ModuleBase::GetTypeName(OtherModuleType)));
	
	int ResAR = EquippableTypes;
	bool AR = ResAR & 1;
	if (AR)
		FullyAutomaticIcon->SetBrushTintColor(FLinearColor::Blue);

	int ResSs = ResAR >> 1;
	bool Ss = ResSs & 1;
	if (Ss)
		SingleShotIcon->SetBrushTintColor(FLinearColor::Blue);

	int ResB3 = ResSs >> 1;
	bool B3 = ResB3 & 1;
	if (B3)
		Burst3Icon->SetBrushTintColor(FLinearColor::Blue);

	int ResShg = ResB3 >> 1;
	bool Shg = ResShg & 1;
	if (Shg)
		ShotgunIcon->SetBrushTintColor(FLinearColor::Blue);

	UpdateSingleStat(DamageText, DamageAmount, DamageDelta, GunStats.Damage, CurrentModuleStats.Damage, OtherModuleStats.Damage);
	UpdateSingleStat(FireRateText, FireRateAmount, FireRateDelta, GunStats.FireRate, CurrentModuleStats.FireRate, OtherModuleStats.FireRate);
	UpdateSingleStat(AccuracyText, AccuracyAmount, AccuracyDelta, GunStats.Accuracy, CurrentModuleStats.Accuracy, OtherModuleStats.Accuracy, true);
	UpdateSingleStat(RecoilText, RecoilAmount, RecoilDelta, GunStats.RecoilRate, CurrentModuleStats.RecoilRate, OtherModuleStats.RecoilRate);
	UpdateSingleStat(ReloadingText, ReloadingAmount, ReloadingDelta, GunStats.ReloadingTime, CurrentModuleStats.ReloadingTime, OtherModuleStats.ReloadingTime, true);
	UpdateSingleStat(ClipSizeText, ClipSizeAmount, ClipSizeDelta, GunStats.ClipSize, CurrentModuleStats.ClipSize, OtherModuleStats.ClipSize);
}

void UOPAG_UWModuleView::UpdateSingleStat(UTextBlock* StatText, UTextBlock* StatAmount, UTextBlock* DeltaText, float BaseAmount, float OldValue, 
	float NewValue, bool bIsInverted) {
	float NewStat = BaseAmount + NewValue;
	float OldStat = BaseAmount + OldValue;
	float Delta = NewStat - OldStat;

	FString DeltaString;

	if (NewStat > OldStat) {
		if (bIsInverted)
			UpdateTextColor(StatText, DeltaText, RedColor);
		else
			UpdateTextColor(StatText, DeltaText, GreenColor);

		DeltaString.AppendChar('+');
	}
	else if (NewStat == OldStat) {
		UpdateTextColor(StatText, DeltaText, WhiteColor);
	}
	else if (NewStat < OldStat) {
		if (bIsInverted)
			UpdateTextColor(StatText, DeltaText, GreenColor);
		else
			UpdateTextColor(StatText, DeltaText, RedColor);
	}

	StatAmount->SetText(UKismetTextLibrary::Conv_FloatToText(NewStat, ERoundingMode::FromZero, false, true, 1, 324, 2, 2));
	
	if(Delta == 0)
		DeltaString.Append("-");
	else
		DeltaString.Append(FString::SanitizeFloat(Delta, 2));

	DeltaText->SetText(FText::FromString(DeltaString));
}

void UOPAG_UWModuleView::UpdateTextColor(UTextBlock* StatText, UTextBlock* DeltaText, FLinearColor Color)
{
	StatText->SetColorAndOpacity(Color);
	DeltaText->SetColorAndOpacity(Color);
}
