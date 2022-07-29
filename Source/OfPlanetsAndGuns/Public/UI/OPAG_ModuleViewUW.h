// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OPAG_ModuleViewUW.generated.h"

class UImage;
class UTextBlock;
/**
 * 
 */
UCLASS()
class OFPLANETSANDGUNS_API UOPAG_ModuleViewUW : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
		UImage* ModuleBackground;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* ModuleName;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* ModuleRarity;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* DamageText;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* FireRateText;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* AccuracyText;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* RecoilText;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* ReloadingText;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* ClipSizeText;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* DamageAmount;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* FireRateAmount;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* AccuracyAmount;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* RecoilAmount;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* ReloadingAmount;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* ClipSizeAmount;

public:
	UFUNCTION()
	void UpdateModuleValues(const FString _ModuleName, const FString _ModuleRarity,  float _Damage, float _FireRate,
			float _Accuracy, float _Recoil, float _Reloading, int _ClipSize);

	void NativeConstruct();
};
