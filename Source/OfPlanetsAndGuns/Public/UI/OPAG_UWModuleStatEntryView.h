// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "OPAG_UWModuleStatEntryView.generated.h"

/**
 * 
 */
class UTextBlock;

UCLASS()
class OFPLANETSANDGUNS_API UOPAG_UWModuleStatEntryView : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

protected:
	// IUserObjectListEntry
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* Name;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* Value;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* Delta;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FLinearColor GreenColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FLinearColor RedColor;
};
