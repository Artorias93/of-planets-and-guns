// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Weapons/Guns/OPAG_GunStats.h"
#include "OPAG_UWNewModuleView.generated.h"

class UImage;
class UTextBlock;
class UListView;
class UHorizontalBox;
class UVerticalBox;
class UTexture2D;

/**
 * 
 */
UCLASS()
class OFPLANETSANDGUNS_API UOPAG_UWNewModuleView : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
		UTextBlock* ModuleRarityText;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* ModuleTypeText;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* ModuleNameText;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* EquippableText;

	UPROPERTY(meta = (BindWidget))
		UListView* StatsListView;

	UPROPERTY(meta = (BindWidget))
		UVerticalBox* AbilityBox;

	UPROPERTY(meta = (BindWidget))
		UHorizontalBox* EquippableTypesBox;

	UPROPERTY(meta = (BindWidget))
		UImage* ModuleBackground; 
	UPROPERTY(meta = (BindWidget))
		UImage* ModuleForeground;

public:
	//Weapons textures
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* FullyAutomaticIconTexture;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* Burst3IconTexture;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* ShotgunIconTexture;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* SingleShotIconTexture;

	//UI Colors
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FLinearColor BackgroundBlue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FLinearColor ForegroundBlue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FLinearColor BackgroundRed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FLinearColor ForegroundRed;

	//UI Font
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FSlateFontInfo AbilityTextFont;


	UFUNCTION()
	void UpdateModuleValues(const FOPAG_GunStats& CurrentModuleStats,const FOPAG_GunStats& OtherModuleStats, 
		const FString OtherModuleName, const FString OtherModuleAbilityName, const EModuleRarity OtherModuleRarity, const EModuleTypes OtherModuleType,
		const int EquippableTypes, const bool CanBeEquipped);

	void NativeConstruct();

	void AddStatEntry(FString StatName, float OldStat, float NewStat, bool bIsInverted = false);
	void AddStatEntry(FString StatName, int OldStat, int NewStat, bool bIsInverted = false);
};
