// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/OPAG_ModuleStatVertItem.h"

UOPAG_ModuleStatVertItem::UOPAG_ModuleStatVertItem()
{

}

void UOPAG_ModuleStatVertItem::Init(FString _DamageText, FString _FireRateText, FString _AccuracyText, 
	FString _StabilityText, FString _ReloadingText, FString _ClipSizeText)
{
	DamageValue = _DamageText;
	FireRateValue = _FireRateText;
	AccuracyValue = _AccuracyText;
	StabilityValue = _StabilityText;
	ReloadingValue = _ReloadingText;
	ClipSizeValue = _ClipSizeText;
}