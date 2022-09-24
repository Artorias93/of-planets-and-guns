// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/OPAG_UWTextDamage.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetTextLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"

void UOPAG_UWTextDamage::NativeConstruct()
{
	Super::NativeConstruct();	
}

void UOPAG_UWTextDamage::Init(float Damage, bool bCrit)
{
	DamageText->SetText(UKismetTextLibrary::Conv_IntToText(FMath::CeilToInt(Damage)));

	if (bCrit)
	{
		UUserWidget::PlayAnimation(DamageTextFadeAnimCrit, 0.f, 1, EUMGSequencePlayMode::Forward, 1, false);
	}
	else
	{
		UUserWidget::PlayAnimation(DamageTextFadeAnim, 0.f, 1, EUMGSequencePlayMode::Forward, 1, false);
	}
}

void UOPAG_UWTextDamage::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::Tick(MyGeometry, InDeltaTime);
}



