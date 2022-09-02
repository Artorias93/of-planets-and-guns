#include "Abilities/OPAG_HealthBuffEffect.h"
#include "Characters/OPAG_AttributeSetBase.h"

UOPAG_HealthBuffEffect::UOPAG_HealthBuffEffect()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayModifierInfo ModifierInfo;
	ModifierInfo.Attribute.SetUProperty(UOPAG_AttributeSetBase::GetMaxHealthAttribute().GetUProperty());
	ModifierInfo.ModifierOp = EGameplayModOp::Multiplicitive;

	const float Percentage = 1 + 0.25f;
	ModifierInfo.ModifierMagnitude = FScalableFloat(Percentage);
	Modifiers.Add(ModifierInfo);
}