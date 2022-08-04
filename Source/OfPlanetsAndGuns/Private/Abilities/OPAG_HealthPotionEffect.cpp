// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/OPAG_HealthPotionEffect.h"
#include "Characters/OPAG_AttributeSetBase.h"

UOPAG_HealthPotionEffect::UOPAG_HealthPotionEffect()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;

	FGameplayEffectModifierMagnitude HealthMagnitude(FScalableFloat(.25f));
	DurationMagnitude = HealthMagnitude;

	FGameplayModifierInfo ModifierInfo;
	ModifierInfo.Attribute.SetUProperty(UOPAG_AttributeSetBase::GetHealthAttribute().GetUProperty());
	ModifierInfo.ModifierOp = EGameplayModOp::Additive;
	ModifierInfo.ModifierMagnitude = FScalableFloat(5.f);
	Modifiers.Add(ModifierInfo);

	Period = FScalableFloat(.025f);
	bExecutePeriodicEffectOnApplication = false;
}