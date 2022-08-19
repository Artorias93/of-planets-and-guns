// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OPAG_UWUpgradeSystemView.h"
#include "UI/OPAG_UWUpgradeSystemItemView.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Managers/OPAG_EventManagerSubsystem.h"
#include "Managers/OPAG_GameInstance.h"
#include "Characters/Player/OPAG_PlayerState.h"
#include "Characters/OPAG_AttributeSetBase.h"


void UOPAG_UWUpgradeSystemView::NativeConstruct()
{
	Super::NativeConstruct();

	UOPAG_EventManagerSubsystem* EMS = GetGameInstance<UOPAG_GameInstance>()->GetEventManager();
	BindEvents();

	if (EMS)
		EMS->RequestPlayerState.Broadcast();
}

void UOPAG_UWUpgradeSystemView::BindEvents()
{
	UOPAG_EventManagerSubsystem* EMS = GetGameInstance<UOPAG_GameInstance>()->GetEventManager();

	if (EMS)
		EMS->ApplyPlayerState.AddDynamic(this, &UOPAG_UWUpgradeSystemView::ApplyPlayerState);
}

void UOPAG_UWUpgradeSystemView::ApplyPlayerState(const FOPAG_PlayerState& StateToApply)
{
	if (BoltsAmountText) {
		BoltsAmountText->SetText(FText::FromString(FString::FromInt(StateToApply.BoltsAmount)));
	}

	UOPAG_UWUpgradeSystemItemView* Entry = CreateWidget<UOPAG_UWUpgradeSystemItemView>(this, UpgradeSystemItemViewWidgetClass);
	EntriesVerticalBox->AddChildToVerticalBox(Entry);
}