// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OPAG_UWPauseView.h"
#include "UI/OPAG_GameplayHUD.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
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