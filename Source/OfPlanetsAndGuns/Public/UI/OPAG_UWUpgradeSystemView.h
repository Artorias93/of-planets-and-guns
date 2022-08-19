// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OPAG_UWUpgradeSystemView.generated.h"

class UOPAG_UWUpgradeSystemItemView;
class UOPAG_EventManagerSubsystem;
class UTextBlock;
class UVerticalBox;
struct FOPAG_PlayerState;

UCLASS()
class OFPLANETSANDGUNS_API UOPAG_UWUpgradeSystemView : public UUserWidget
{
	GENERATED_BODY()

	void BindEvents();

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* BoltsAmountText;
	UPROPERTY(meta = (BindWidget))
		class UVerticalBox* EntriesVerticalBox;

private:
	UPROPERTY()
		class UOPAG_UWUpgradeSystemItemView* UpgradeSystemItemWidget;
	UPROPERTY(EditAnywhere, Category = "Widgets")
		TSubclassOf<UOPAG_UWUpgradeSystemItemView> UpgradeSystemItemViewWidgetClass;

public:
	UFUNCTION()
	void ApplyPlayerState(const FOPAG_PlayerState& StateToApply);
};
