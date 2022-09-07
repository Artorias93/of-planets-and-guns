// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OPAG_UWPlayerStatsView.generated.h"

/**
 * 
 */
class UTextBlock;
class UProgressBar;
class UImage;
class UHorizontalBox;
class UHorizontalBoxSlot;
class UTexture2D;

UCLASS()
class OFPLANETSANDGUNS_API UOPAG_UWPlayerStatsView : public UUserWidget
{
	GENERATED_BODY()

	void BindEvents();

	UPROPERTY(meta = (BindWidget))
		UProgressBar* HealthBar;
	UPROPERTY(meta = (BindWidget))
		UProgressBar* ShieldBar;

	UPROPERTY(meta = (BindWidget))
		UHorizontalBox* DashBox; 

	TArray<UProgressBar*> Dashes;
	UProgressBar* LastRechargingDashBoxSlot;
	int32 NextToBeUsedDashBoxSlot;

public:
	UFUNCTION()
		void UpdateHealthBar(const float CurrentValue, const float MaxValue);
	UFUNCTION()
		void UpdateShieldBar(const float CurrentValue, const float MaxValue);

	UFUNCTION()
	void DashInit();
	UFUNCTION()
	void IncrementDashAmount();
	UFUNCTION()
	void DecrementDashAmount();

	void NativeConstruct();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		UTexture2D* DashIconTexture;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		UTexture2D* PotionsIconTexture;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		UTexture2D* BoltsIconTexture;

protected:
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

};
