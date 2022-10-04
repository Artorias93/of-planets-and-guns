// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OPAG_UWPauseView.h"
#include "UI/OPAG_GameplayHUD.h"
#include "UI/OPAG_ModuleStatVertItem.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/ListView.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetTextLibrary.h"
#include "Weapons/Guns/OPAG_GunStats.h"
#include "Weapons/Modules/OPAG_ModuleBase.h"
#include "Managers/OPAG_GameInstance.h"
#include "Managers/OPAG_EventManagerSubsystem.h"

void UOPAG_UWPauseView::NativeConstruct()
{
	Super::NativeConstruct();

	BindEvents();

	ContinueButton->OnClicked.AddUniqueDynamic(this, &UOPAG_UWPauseView::ContinueButtonClicked);
	OptionsButton->OnClicked.AddUniqueDynamic(this, &UOPAG_UWPauseView::OptionsButtonClicked);
	ExitButton->OnClicked.AddUniqueDynamic(this, &UOPAG_UWPauseView::ExitButtonClicked);
}

void UOPAG_UWPauseView::BindEvents()
{
	UOPAG_EventManagerSubsystem* EMS = GetGameInstance<UOPAG_GameInstance>()->GetEventManager();

	if (EMS) 
	{
		EMS->GoldChanged.AddUniqueDynamic(this, &UOPAG_UWPauseView::UpdateGoldValue);
	}
}

void UOPAG_UWPauseView::ContinueButtonClicked()
{
	AOPAG_GameplayHUD* MyHUD = Cast<AOPAG_GameplayHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if(MyHUD)
		MyHUD->HidePauseView();
}

void UOPAG_UWPauseView::OptionsButtonClicked()
{
	AOPAG_GameplayHUD* MyHUD = Cast<AOPAG_GameplayHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (MyHUD)
		MyHUD->ShowOptionsView();
}

void UOPAG_UWPauseView::ExitButtonClicked()
{
	AOPAG_GameplayHUD* MyHUD = Cast<AOPAG_GameplayHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (MyHUD)
		MyHUD->GoToMenu();
}

void UOPAG_UWPauseView::UpdateGoldValue(const int Value)
{
	if (GoldAmountText && Value >= 0) 
	{
		GoldAmountText->SetText(FText::FromString(FString::FromInt(Value)));
	}
}

void UOPAG_UWPauseView::UpdateGunValues(const FOPAG_GunStats GunStats,
	const FString GunName, const FString GunFireType, const TArray<AOPAG_ModuleBase*> Modules)
{
	if (DamageAmount)
		DamageAmount->SetText(UKismetTextLibrary::Conv_FloatToText(GunStats.Damage, ERoundingMode::FromZero, false, true, 1, 324, 2, 2));
	if (FireRateAmount)
		FireRateAmount->SetText(UKismetTextLibrary::Conv_FloatToText(GunStats.FireRate, ERoundingMode::FromZero, false, true, 1, 324, 2, 2));
	if (AccuracyAmount)
		AccuracyAmount->SetText(UKismetTextLibrary::Conv_FloatToText(GunStats.Accuracy, ERoundingMode::FromZero, false, true, 1, 324, 2, 2));
	if (StabilityAmount)
		StabilityAmount->SetText(UKismetTextLibrary::Conv_FloatToText(GunStats.Stability, ERoundingMode::FromZero, false, true, 1, 324, 2, 2));
	if (ReloadingAmount)
		ReloadingAmount->SetText(UKismetTextLibrary::Conv_FloatToText(GunStats.ReloadingTime, ERoundingMode::FromZero, false, true, 1, 324, 2, 2));
	if (ClipSizeAmount)
		ClipSizeAmount->SetText(UKismetTextLibrary::Conv_FloatToText(GunStats.ClipSize, ERoundingMode::FromZero, false, true, 1, 324, 2, 2));

	for (AOPAG_ModuleBase* module : Modules) 
	{
		UOPAG_ModuleStatVertItem* item = NewObject<UOPAG_ModuleStatVertItem>(UOPAG_ModuleStatVertItem::StaticClass());
		if (item) 
		{
			FOPAG_GunStats ModuleStats;
			FString ModuleName;

			if(module)
				ModuleStats = module->GetStats();

			if (module) 
			{
				ModuleName = module->GetTypeName(module->GetType());
			}
			else
				ModuleName = "Accessory";

			if (ModuleName.Equals("Sight") && !(GunFireType.Equals("SingleShot")))
			{
				continue;
			}

			FString DamageString = ModuleStats.Damage == 0.f ? "-" : FString::Printf(TEXT("%.2f"), ModuleStats.Damage);
			FString FireRateString = ModuleStats.FireRate == 0.f ? "-" : FString::Printf(TEXT("%.2f"), ModuleStats.FireRate);
			FString AccuracyString = ModuleStats.Accuracy == 0.f ? "-" : FString::Printf(TEXT("%.2f"), ModuleStats.Accuracy);
			FString StabilityString = ModuleStats.Stability == 0.f ? "-" : FString::Printf(TEXT("%.2f"), ModuleStats.Stability);
			FString ReloadingTimeString = ModuleStats.ReloadingTime == 0.f ? "-" : FString::Printf(TEXT("%.2f"), ModuleStats.ReloadingTime);
			FString ClipSizeString = ModuleStats.ClipSize == 0.f ? "-" : FString::Printf(TEXT("%.2f"), ModuleStats.ClipSize);

			item->Init(
				ModuleName,
				DamageString,
				FireRateString,
				AccuracyString,
				StabilityString,
				ReloadingTimeString,
				ClipSizeString
			);

			StatsListView->AddItem(item);
			item->MarkPendingKill();
		}
	}
}