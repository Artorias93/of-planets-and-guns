// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "OPAG_HealthPotionEffect.generated.h"

/**
 * 
 */
UCLASS()
class OFPLANETSANDGUNS_API UOPAG_HealthPotionEffect : public UGameplayEffect
{
	GENERATED_BODY()

	UOPAG_HealthPotionEffect();

private:
	UPROPERTY(Category = "OPAG | Ability | Potion", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float HealthModifierMagnitudeAmount = 5.f;

	UPROPERTY(Category = "OPAG | Ability | Potion", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float HealthMagnitudeAmount = .25f;

	UPROPERTY(Category = "OPAG | Ability | Potion", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float HealthPeriodAmount = .025f;
};
