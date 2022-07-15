// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OPAG_UWGunView.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Managers/OPAG_GameInstance.h"
#include "Managers/OPAG_EventManagerSubsystem.h"

void UOPAG_UWGunView::NativeConstruct() {
	BindEvents();
}

void UOPAG_UWGunView::BindEvents()
{
	UOPAG_EventManagerSubsystem* EMS = GetGameInstance<UOPAG_GameInstance>()->GetEventManager();
	EMS->TotalAmmoChanged.AddDynamic(this, &UOPAG_UWGunView::UpdateTotalAmmoText);
	EMS->CurrentAmmoChanged.AddDynamic(this, &UOPAG_UWGunView::UpdateCurrentAmmoText);
}

void UOPAG_UWGunView::UpdateCurrentAmmoText(const int CurrentValue)
{
	if (CurrentAmmoText && CurrentValue >= 0) {
		CurrentAmmoText->SetText(FText::FromString(FString::FromInt(CurrentValue)));
	}
}

void UOPAG_UWGunView::UpdateTotalAmmoText(const int CurrentValue)
{
	if (TotalAmmoText && CurrentValue >= 0) {
		TotalAmmoText->SetText(FText::FromString(FString::FromInt(CurrentValue)));
	}
}
