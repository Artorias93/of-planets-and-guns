// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OPAG_UWOptionsView.h"
#include "UI/OPAG_GameplayHUD.h"
#include "Components/Slider.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/OPAG_GameInstance.h"
#include "Managers/OPAG_EventManagerSubsystem.h"
#include "GameFramework/GameUserSettings.h"

void UOPAG_UWOptionsView::NativeConstruct()
{
	Super::NativeConstruct();

	//BindEvents();

	UGameUserSettings* GameUserSettings = Cast<UGameUserSettings>(GEngine->GetGameUserSettings());

	if (GameUserSettings)
	{
		int32 OverallScalabilitySettingsValue = GameUserSettings->GetOverallScalabilityLevel();
		GraphicsSlider->SetValue(OverallScalabilitySettingsValue);
	}

	BackButton->OnClicked.AddUniqueDynamic(this, &UOPAG_UWOptionsView::BackButtonClicked);
	GraphicsSlider->OnValueChanged.AddUniqueDynamic(this, &UOPAG_UWOptionsView::GraphicsSliderChanged);
}

void UOPAG_UWOptionsView::BackButtonClicked()
{
	AOPAG_GameplayHUD* MyHUD = Cast<AOPAG_GameplayHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (MyHUD)
		MyHUD->HideOptionsView();
}

void UOPAG_UWOptionsView::GraphicsSliderChanged(float CurrentValue)
{
	UOPAG_EventManagerSubsystem* EMS = GetGameInstance<UOPAG_GameInstance>()->GetEventManager();

	if (EMS)
	{
		EMS->QualitySettingsChanged.Broadcast(CurrentValue);
	}
}

