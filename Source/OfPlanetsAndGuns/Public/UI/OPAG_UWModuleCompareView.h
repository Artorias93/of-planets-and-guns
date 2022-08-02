// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OPAG_UWModuleCompareView.generated.h"

/**
 * 
 */
class UImage;
class UTextBlock;
class UOPAG_ModuleViewUW;

UCLASS()
class OFPLANETSANDGUNS_API UOPAG_UWModuleCompareView : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
		UOPAG_ModuleViewUW* CurrentModuleWidget;
	UPROPERTY(meta = (BindWidget))
		UOPAG_ModuleViewUW* OtherModuleWidget;

public:
	UFUNCTION()
	void UpdateModulesValues(const FOPAG_GunStats& CurrentModule, const FOPAG_GunStats& OtherModule,
			const FString CurrentModuleName, const EModuleRarity CurrentModuleRarity, const FString OtherModuleName, const EModuleRarity OtherModuleRarity);

	void NativeConstruct();
};
