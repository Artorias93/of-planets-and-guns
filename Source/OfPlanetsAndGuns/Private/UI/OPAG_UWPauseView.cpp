// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OPAG_UWPauseView.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "UI/OPAG_GameplayHUD.h"


void UOPAG_UWPauseView::NativeConstruct()
{
	Super::NativeConstruct();
	ContinueButton->OnClicked.AddDynamic(this, &UOPAG_UWPauseView::ContinueButtonClicked);
	OptionsButton->OnClicked.AddDynamic(this, &UOPAG_UWPauseView::OptionsButtonClicked);
	ExitButton->OnClicked.AddDynamic(this, &UOPAG_UWPauseView::ExitButtonClicked);
}

void UOPAG_UWPauseView::ContinueButtonClicked()
{
	ContinueButton->OnClicked.RemoveDynamic(this, &UOPAG_UWPauseView::ContinueButtonClicked);
	//How will i get it?
	AOPAG_GameplayHUD* MyHUD = Cast<AOPAG_GameplayHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if(MyHUD)
		MyHUD->HidePauseView();
}

void UOPAG_UWPauseView::OptionsButtonClicked()
{
	OptionsButton->OnClicked.RemoveDynamic(this, &UOPAG_UWPauseView::OptionsButtonClicked);
}

void UOPAG_UWPauseView::ExitButtonClicked()
{
	ExitButton->OnClicked.RemoveDynamic(this, &UOPAG_UWPauseView::ExitButtonClicked);
}
