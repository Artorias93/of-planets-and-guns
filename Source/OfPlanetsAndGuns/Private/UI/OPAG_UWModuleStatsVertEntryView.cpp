// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OPAG_UWModuleStatsVertEntryView.h"
#include "Components/TextBlock.h"

void UOPAG_UWModuleStatsVertEntryView::NativeConstruct()
{
	Super::NativeConstruct();

	DamageText->SetText(FText::FromString("999"));
	AccuracyText->SetText(FText::FromString("999"));
	FireRateText->SetText(FText::FromString("999"));
	ReloadingText->SetText(FText::FromString("999"));
	ClipSizeText->SetText(FText::FromString("999"));
	StabilityText->SetText(FText::FromString("999"));
}