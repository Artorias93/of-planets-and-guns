// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "OPAG_UWUpgradeSystemView.generated.h"

class UOPAG_UWUpgradeSystemItemView;

UCLASS()
class OFPLANETSANDGUNS_API UOPAG_UWUpgradeSystemView : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()	

protected:
	virtual void NativeConstruct() override;
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

private:
	UPROPERTY()
		class UOPAG_UWUpgradeSystemView* UpgradeSystemWidget;
	UPROPERTY(EditAnywhere, Category = "Widgets")
		TSubclassOf<UOPAG_UWUpgradeSystemItemView> UpgradeSystemItemViewWidgetClass;
};
