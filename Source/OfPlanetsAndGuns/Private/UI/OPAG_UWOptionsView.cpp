// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OPAG_UWOptionsView.h"
#include "UI/OPAG_GameplayHUD.h"
#include "Components/Slider.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/OPAG_GameInstance.h"
#include "Managers/OPAG_EventManagerSubsystem.h"
#include "Managers/OPAG_SoundManagerSubsystem.h"
#include "GameFramework/GameUserSettings.h"
#include "UI/OPAG_MainMenuHUD.h"

void UOPAG_UWOptionsView::NativeConstruct()
{
	Super::NativeConstruct();

	//BindEvents();
	
	EMS = GetGameInstance<UOPAG_GameInstance>()->GetEventManager();

	UGameUserSettings* GameUserSettings = Cast<UGameUserSettings>(GEngine->GetGameUserSettings());

	if (GameUserSettings)
	{
		int32 OverallScalabilitySettingsValue = GameUserSettings->GetOverallScalabilityLevel();
		GraphicsSlider->SetValue(OverallScalabilitySettingsValue);
	}

	SMS = GetGameInstance<UOPAG_GameInstance>()->GetSoundManager();

	if (SMS) 
	{
		float MasterValue = 0.f; //Get  value from FMod
		MasterSlider->SetValue(MasterValue);
		float MusicValue = 0.f; //Get  value from FMod
		MusicSlider->SetValue(MusicValue);
		float SFXValue = 0.f; //Get  value from FMod
		SFXSlider->SetValue(SFXValue);
		float AmbientValue = 0.f; //Get  value from FMod
		AmbientSlider->SetValue(AmbientValue);
		float SystemValue = 0.f; //Get  value from FMod
		SystemSlider->SetValue(SystemValue);
	}

	BackButton->OnClicked.AddUniqueDynamic(this, &UOPAG_UWOptionsView::BackButtonClicked);

	GraphicsSlider->OnValueChanged.AddUniqueDynamic(this, &UOPAG_UWOptionsView::GraphicsSliderChanged);

	MasterSlider->OnValueChanged.AddUniqueDynamic(this, &UOPAG_UWOptionsView::MasterSliderChanged);
	MusicSlider->OnValueChanged.AddUniqueDynamic(this, &UOPAG_UWOptionsView::MusicSliderChanged);
	SFXSlider->OnValueChanged.AddUniqueDynamic(this, &UOPAG_UWOptionsView::SFXSliderChanged);
	AmbientSlider->OnValueChanged.AddUniqueDynamic(this, &UOPAG_UWOptionsView::AmbientSliderChanged);
	SystemSlider->OnValueChanged.AddUniqueDynamic(this, &UOPAG_UWOptionsView::SystemSliderChanged);
}

void UOPAG_UWOptionsView::BackButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Back Button Clicked"));
	if (AOPAG_GameplayHUD* const MyHUD = Cast<AOPAG_GameplayHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
	{
		MyHUD->HideOptionsView();
		return;
	}
	if(const AOPAG_MainMenuHUD* const MenuHUD = Cast<AOPAG_MainMenuHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
	{
		UE_LOG(LogTemp, Warning, TEXT("In cast body"));
		MenuHUD->SequencePlaySelection(Options, MainMenu);
	}
}

void UOPAG_UWOptionsView::GraphicsSliderChanged(float CurrentValue)
{
	if (EMS)
	{
		EMS->QualitySettingsChanged.Broadcast(CurrentValue);
	}
}

void UOPAG_UWOptionsView::MasterSliderChanged(float CurrentValue)
{
	if (EMS)
	{
		EMS->MasterSliderChanged.Broadcast(CurrentValue);
	}
}

void UOPAG_UWOptionsView::MusicSliderChanged(float CurrentValue)
{
	if (EMS)
	{
		EMS->MusicSliderChanged.Broadcast(CurrentValue);
	}
}

void UOPAG_UWOptionsView::SFXSliderChanged(float CurrentValue)
{
	if (EMS)
	{
		EMS->SFXSliderChanged.Broadcast(CurrentValue);
	}
}

void UOPAG_UWOptionsView::AmbientSliderChanged(float CurrentValue)
{
	if (EMS)
	{
		EMS->AmbientSliderChanged.Broadcast(CurrentValue);
	}
}

void UOPAG_UWOptionsView::SystemSliderChanged(float CurrentValue)
{
	if (EMS)
	{
		EMS->SystemSliderChanged.Broadcast(CurrentValue);
	}
}