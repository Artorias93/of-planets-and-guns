// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OPAG_UWTextDamage.generated.h"

/**
 * 
 */
UCLASS()
class OFPLANETSANDGUNS_API UOPAG_UWTextDamage : public UUserWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct();
	void Init(float Damage, bool bCrit);
	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;
protected:
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* DamageText;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* DamageTextFadeAnim;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* DamageTextFadeAnimCrit;

	UPROPERTY()
		float IncomingDamage;


	
};
