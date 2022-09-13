// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OPAG_UWOptionsView.h"
#include "UI/OPAG_GameplayHUD.h"
#include "Components/Slider.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/OPAG_GameInstance.h"
#include "Managers/OPAG_EventManagerSubsystem.h"

void UOPAG_UWOptionsView::NativeConstruct()
{
	Super::NativeConstruct();

	//BindEvents();

	BackButton->OnClicked.AddUniqueDynamic(this, &UOPAG_UWOptionsView::BackButtonClicked);
}

void UOPAG_UWOptionsView::BackButtonClicked()
{
	AOPAG_GameplayHUD* MyHUD = Cast<AOPAG_GameplayHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (MyHUD)
		MyHUD->HideOptionsView();
}

