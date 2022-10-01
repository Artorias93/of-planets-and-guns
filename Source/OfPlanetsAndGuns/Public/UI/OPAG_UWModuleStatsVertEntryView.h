// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "OPAG_UWModuleStatsVertEntryView.generated.h"

/**
 * 
 */
class UTextBlock;

UCLASS()
class OFPLANETSANDGUNS_API UOPAG_UWModuleStatsVertEntryView : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

protected:
	// IUserObjectListEntry
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* DamageText;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* FireRateText;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* AccuracyText;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* StabilityText;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* ReloadingText;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* ClipSizeText;
};