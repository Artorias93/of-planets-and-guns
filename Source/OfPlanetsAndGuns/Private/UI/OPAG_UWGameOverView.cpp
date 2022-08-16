// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OPAG_UWGameOverView.h"

#include "Components/Button.h"
#include "Managers/OPAG_EventManagerSubsystem.h"
#include "Managers/OPAG_GameInstance.h"


void UOPAG_UWGameOverView::NativeConstruct()
{
	Super::NativeConstruct();
	ExitButton->OnClicked.AddDynamic(this, &UOPAG_UWGameOverView::ExitButtonClicked);
}

void UOPAG_UWGameOverView::ExitButtonClicked()
{
	auto GI = GetGameInstance<UOPAG_GameInstance>();
	if (GI)
		GI->GetEventManager()->LoadMainMenu.Broadcast();
}

