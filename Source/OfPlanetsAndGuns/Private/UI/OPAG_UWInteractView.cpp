// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OPAG_UWInteractView.h"
#include "Components/TextBlock.h"
/*
#include "Managers/OPAG_GameInstance.h"
#include "Managers/OPAG_EventManagerSubsystem.h"
*/

void UOPAG_UWInteractView::NativeConstruct()
{
	Super::NativeConstruct();
	//BindEvents();
}

void UOPAG_UWInteractView::BindEvents()
{

}

void UOPAG_UWInteractView::UpdateInteractText(bool bCanInteract, const FString InteractTextParam)
{
	InteractText->SetText(FText::FromString(InteractTextParam));
	InteractText->SetOpacity(1.0f);

	if (!bCanInteract) {
		InteractText->SetOpacity(0.4f);
	}
}
