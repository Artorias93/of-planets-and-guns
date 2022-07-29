// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OPAG_ModuleViewUW.h"
#include "Components/TextBlock.h"


void UOPAG_ModuleViewUW::NativeConstruct()
{

}

void UOPAG_ModuleViewUW::UpdateModuleValues(const FString _ModuleName, const FString _ModuleRarity, float _Damage, float _FireRate,
	float _Accuracy, float _Recoil, float _Reloading, int _ClipSize)
{
	ModuleName->SetText(FText::FromString(_ModuleName));
	ModuleRarity->SetText(FText::FromString(_ModuleRarity));
	DamageAmount->SetText(FText::FromString(FString::SanitizeFloat(_Damage)));
	FireRateAmount->SetText(FText::FromString(FString::SanitizeFloat(_FireRate)));
	AccuracyAmount->SetText(FText::FromString(FString::SanitizeFloat(_Accuracy)));
	RecoilAmount->SetText(FText::FromString(FString::SanitizeFloat(_Recoil)));
	ReloadingAmount->SetText(FText::FromString(FString::SanitizeFloat(_Reloading)));
	ClipSizeAmount->SetText(FText::FromString(FString::FromInt(_ClipSize)));
}
