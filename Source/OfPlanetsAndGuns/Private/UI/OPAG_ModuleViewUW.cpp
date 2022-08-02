// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OPAG_ModuleViewUW.h"
#include "Weapons/Guns/OPAG_GunBase.h"
#include "Components/TextBlock.h"


void UOPAG_ModuleViewUW::NativeConstruct()
{
	Super::NativeConstruct();
}

void UOPAG_ModuleViewUW::UpdateModuleValues(const FOPAG_GunStats& Stats, const FString Name, const EModuleRarity Rarity)
{
	/*
	ModuleName->SetText(FText::FromString(_ModuleName));
	ModuleRarity->SetText(FText::FromString(_ModuleRarity));
	DamageAmount->SetText(FText::FromString(FString::SanitizeFloat(_Damage)));
	FireRateAmount->SetText(FText::FromString(FString::SanitizeFloat(_FireRate)));
	AccuracyAmount->SetText(FText::FromString(FString::SanitizeFloat(_Accuracy)));
	RecoilAmount->SetText(FText::FromString(FString::SanitizeFloat(_Recoil)));
	ReloadingAmount->SetText(FText::FromString(FString::SanitizeFloat(_Reloading)));
	ClipSizeAmount->SetText(FText::FromString(FString::FromInt(_ClipSize)));
	*/

	ModuleName->SetText(FText::FromString(Name));
	ModuleRarity->SetText(FText::FromString("No Method - Do it"));
	DamageAmount->SetText(FText::FromString(FString::SanitizeFloat(Stats.Damage)));
	FireRateAmount->SetText(FText::FromString(FString::SanitizeFloat(Stats.FireRate)));
	AccuracyAmount->SetText(FText::FromString(FString::SanitizeFloat(Stats.Accuracy)));
	RecoilAmount->SetText(FText::FromString(FString::SanitizeFloat(Stats.RecoilRate)));
	ReloadingAmount->SetText(FText::FromString(FString::SanitizeFloat(Stats.ReloadingTime)));
	ClipSizeAmount->SetText(FText::FromString(FString::FromInt(Stats.ClipSize)));

}
