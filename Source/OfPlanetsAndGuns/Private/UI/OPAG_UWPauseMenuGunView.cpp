// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OPAG_UWPauseMenuGunView.h"
#include "Weapons/Guns/OPAG_GunBase.h"
#include "Kismet/KismetTextLibrary.h"
#include "Weapons/Modules/OPAG_ModuleBase.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/OPAG_GameInstance.h"
#include "Managers/OPAG_EventManagerSubsystem.h"

void UOPAG_UWPauseMenuGunView::NativeConstruct()
{
	Super::NativeConstruct();
	BindEvents();
}

void UOPAG_UWPauseMenuGunView::BindEvents()
{
	UOPAG_EventManagerSubsystem* EMS = GetGameInstance<UOPAG_GameInstance>()->GetEventManager();

	if (EMS)
	{
		//Mettere evento che manda parametri dell'arma del giocatore
		EMS->PauseMenuGunChanged.AddUniqueDynamic(this, &UOPAG_UWPauseMenuGunView::UpdateGunValues);
	}
}

void UOPAG_UWPauseMenuGunView::UpdateGunValues(const FOPAG_GunStats GunStats, const FString MyGunName, const FString GunFireType)
{
	GunName->SetText(FText::FromString(MyGunName));
	GunType->SetText(FText::FromString(GunFireType));

	UpdateSingleStat(DamageText, DamageAmount,GunStats.Damage);
	UpdateSingleStat(FireRateText, FireRateAmount, GunStats.FireRate);
	UpdateSingleStat(AccuracyText, AccuracyAmount,GunStats.Accuracy);
	UpdateSingleStat(StabilityText, StabilityAmount, GunStats.Stability);
	UpdateSingleStat(ReloadingText, ReloadingAmount, GunStats.ReloadingTime);
	UpdateSingleStat(ClipSizeText, ClipSizeAmount, GunStats.ClipSize);
}

void UOPAG_UWPauseMenuGunView::UpdateSingleStat(UTextBlock* StatText, UTextBlock* StatAmount, float Value)
{
	StatAmount->SetText(UKismetTextLibrary::Conv_FloatToText(Value, ERoundingMode::FromZero, false, true, 1, 324, 2, 2));
}