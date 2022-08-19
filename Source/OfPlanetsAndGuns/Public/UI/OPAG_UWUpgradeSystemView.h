// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OPAG_UWUpgradeSystemView.generated.h"

class UOPAG_UWUpgradeSystemItemView;
class UOPAG_EventManagerSubsystem;
class UTextBlock;
struct FOPAG_PlayerState;

UCLASS()
class OFPLANETSANDGUNS_API UOPAG_UWUpgradeSystemView : public UUserWidget
{
	GENERATED_BODY()

	void BindEvents();

	//FOPAG_PlayerState* tempPlayerState;

	//UOPAG_EventManagerSubsystem* const EMS;

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
		class UListView* InventoryListView;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* BoltsAmountText;

private:
	UPROPERTY()
		class UOPAG_UWUpgradeSystemView* UpgradeSystemWidget;
	UPROPERTY(EditAnywhere, Category = "Widgets")
		TSubclassOf<UOPAG_UWUpgradeSystemItemView> UpgradeSystemItemViewWidgetClass;

public:
	UFUNCTION()
	void ApplyPlayerState(const FOPAG_PlayerState& StateToApply);
};
