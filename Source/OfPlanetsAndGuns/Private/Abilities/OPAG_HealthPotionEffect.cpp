// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/OPAG_HealthPotionEffect.h"
#include "Characters/OPAG_AttributeSetBase.h"

UOPAG_HealthPotionEffect::UOPAG_HealthPotionEffect()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;
	FScalableFloat HealthScalableFloat;
	HealthScalableFloat.SetValue(.25f);
	FGameplayEffectModifierMagnitude HealthMagnitude(HealthScalableFloat);
	DurationMagnitude = HealthMagnitude;
	FGameplayModifierInfo ModifierInfo;
	ModifierInfo.Attribute = UOPAG_AttributeSetBase::GetHealthAttribute();
	ModifierInfo.ModifierOp = EGameplayModOp::Additive;

	FScalableFloat ModifierScalableFloat;
	ModifierScalableFloat.SetValue(1.f);
	ModifierInfo.Magnitude = ModifierScalableFloat;

	TArray<FGameplayModifierInfo> ModifiersInfo;
	Modifiers.Add(ModifierInfo);

	Modifiers = ModifiersInfo;

	FScalableFloat PeriodScalableFloat;
	PeriodScalableFloat.SetValue(.025f);
	Period = PeriodScalableFloat;
}