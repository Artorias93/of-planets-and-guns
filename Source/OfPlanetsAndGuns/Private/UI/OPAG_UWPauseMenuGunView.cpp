// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OPAG_UWPauseMenuGunView.h"
#include "UI/OPAG_UWModuleStatsVertEntryView.h"
#include "Kismet/KismetTextLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Weapons/Modules/OPAG_ModuleBase.h"
#include "Weapons/Guns/OPAG_GunBase.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"
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
		//EMS->PauseMenuGunChanged.AddUniqueDynamic(this, &UOPAG_UWPauseMenuGunView::UpdateGunValues);

		//Remove all previous icons and stats
		StatsHorizontalBox->ClearChildren();

		//UOPAG_UWModuleStatsVertEntryView* const StatsVertEntry = WidgetTree->ConstructWidget<UOPAG_UWModuleStatsVertEntryView>();
		//StatsListView->AddChildToHorizontalBox(StatsVertEntry);
	}
}

void UOPAG_UWPauseMenuGunView::UpdateGunValues(const FOPAG_GunStats GunStats, const FString MyGunName, const FString GunFireType)
{
	GunName->SetText(FText::FromString(MyGunName));
	GunType->SetText(FText::FromString(GunFireType));

	UpdateSingleStat(DamageAmount,GunStats.Damage);
	UpdateSingleStat(FireRateAmount, GunStats.FireRate);
	UpdateSingleStat(AccuracyAmount,GunStats.Accuracy);
	UpdateSingleStat(StabilityAmount, GunStats.Stability);
	UpdateSingleStat(ReloadingAmount, GunStats.ReloadingTime);
	UpdateSingleStat(ClipSizeAmount, GunStats.ClipSize);
}

void UOPAG_UWPauseMenuGunView::UpdateSingleStat(UTextBlock* StatAmount, float Value)
{
	StatAmount->SetText(UKismetTextLibrary::Conv_FloatToText(Value, ERoundingMode::FromZero, false, true, 1, 324, 2, 2));
}