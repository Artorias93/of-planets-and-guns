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
	Super::NativeConstruct();
	BindEvents();
}

void UOPAG_UWGunView::BindEvents()
{
	UOPAG_EventManagerSubsystem* const EMS = GetGameInstance<UOPAG_GameInstance>()->GetEventManager();

	if (EMS) {
		EMS->TotalAmmoChanged.AddDynamic(this, &UOPAG_UWGunView::UpdateTotalAmmoText);
		EMS->CurrentAmmoChanged.AddDynamic(this, &UOPAG_UWGunView::UpdateCurrentAmmoText);
		EMS->ColorAmmoChanged.AddDynamic(this, &UOPAG_UWGunView::UpdateAmmoColor);
		EMS->SwapGunTexture.AddDynamic(this, &UOPAG_UWGunView::SwapGun);
	}
}

void UOPAG_UWGunView::UpdateCurrentAmmoText(const int CurrentValue)
{
	if (CurrentAmmoText && CurrentValue >= 0) {
		CurrentAmmoText->SetText(FText::FromString(FString::FromInt(CurrentValue)));
	}
}

void UOPAG_UWGunView::UpdateTotalAmmoText(const int CurrentValue)
{
	if (TotalAmmoText && CurrentValue >= 0) 
	{
		//99999 is the default AmmoMax pistol value ( set in Blueprint )
		if(CurrentValue >= 99999)
		{
			TotalAmmoText->SetText(FText::FromString("-"));
		}
		else
		{
			TotalAmmoText->SetText(FText::FromString(FString::FromInt(CurrentValue)));
		}
	}
}

void UOPAG_UWGunView::UpdateAmmoColor(const FLinearColor ColorParam)
{
	if (TotalAmmoText && CurrentAmmoText) {
		CurrentAmmoText->SetColorAndOpacity(ColorParam);
		TotalAmmoText->SetColorAndOpacity(ColorParam);
	}
}

void UOPAG_UWGunView::SwapGun(EFireTypes CurrentType)
{
	switch (CurrentType)
	{
	case EFireTypes::FullyAutomatic:
		GunIcon->SetBrushFromTexture(RifleTexture2D);
		GunIcon->SetVisibility(ESlateVisibility::Visible);
		HandgunIcon->SetVisibility(ESlateVisibility::Hidden);
		break;
	case EFireTypes::Pistol:
		HandgunIcon->SetVisibility(ESlateVisibility::Visible);
		GunIcon->SetVisibility(ESlateVisibility::Hidden);
		break;
	default:
		break;
	}
}
