// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OPAG_UWUpgradeSystemView.generated.h"

class UOPAG_UWUpgradeSystemItemView;

UCLASS()
class OFPLANETSANDGUNS_API UOPAG_UWUpgradeSystemView : public UUserWidget
{
	GENERATED_BODY()	

protected:
	virtual void NativeConstruct() override;

	/*UPROPERTY(meta = (BindWidget))
		class UListView* InventoryListView;*/

private:
	UPROPERTY()
		class UOPAG_UWUpgradeSystemView* UpgradeSystemWidget;
	UPROPERTY(EditAnywhere, Category = "Widgets")
		TSubclassOf<UOPAG_UWUpgradeSystemItemView> UpgradeSystemItemViewWidgetClass;
};
