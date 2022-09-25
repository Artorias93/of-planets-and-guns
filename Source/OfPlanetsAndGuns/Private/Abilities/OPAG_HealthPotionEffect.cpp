// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/OPAG_HealthPotionEffect.h"
#include "Characters/OPAG_AttributeSetBase.h"

UOPAG_HealthPotionEffect::UOPAG_HealthPotionEffect()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;

	FScalableFloat HealthScalableFloatAmount = FScalableFloat(HealthModifierMagnitudeAmount);
	FGameplayEffectModifierMagnitude HealthModifierMagnitude(HealthScalableFloatAmount);
	DurationMagnitude = HealthModifierMagnitude;

	FGameplayModifierInfo ModifierInfo;
	ModifierInfo.Attribute.SetUProperty(UOPAG_AttributeSetBase::GetHealthAttribute().GetUProperty());
	ModifierInfo.ModifierOp = EGameplayModOp::Additive;
	ModifierInfo.ModifierMagnitude = FScalableFloat(HealthModifierMagnitudeAmount);
	Modifiers.Add(ModifierInfo);

	Period = FScalableFloat(HealthPeriodAmount);
	bExecutePeriodicEffectOnApplication = false;
}