// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OPAG_UWModuleStatsVertEntryView.h"
#include "UI/OPAG_ModuleStatVertItem.h"
#include "Components/TextBlock.h"

void UOPAG_UWModuleStatsVertEntryView::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	UOPAG_ModuleStatVertItem* Item = Cast<UOPAG_ModuleStatVertItem>(ListItemObject);

	if (Item)
	{
		DamageText->SetText(FText::FromString(Item->DamageValue));
		FireRateText->SetText(FText::FromString(Item->FireRateValue));
		AccuracyText->SetText(FText::FromString(Item->AccuracyValue));
		StabilityText->SetText(FText::FromString(Item->StabilityValue));
		ReloadingText->SetText(FText::FromString(Item->ReloadingValue));
		ClipSizeText->SetText(FText::FromString(Item->ClipSizeValue));
	}
}