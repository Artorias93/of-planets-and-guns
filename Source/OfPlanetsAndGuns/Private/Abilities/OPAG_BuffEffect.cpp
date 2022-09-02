#include "Abilities/OPAG_BuffEffect.h"

UOPAG_BuffEffect::UOPAG_BuffEffect()
{
	DurationPolicy = EGameplayEffectDurationType::Infinite;
}

void UOPAG_BuffEffect::SetModifierInfo(FGameplayModifierInfo ModifierInfo)
{
	Modifiers.Add(ModifierInfo);
}

void UOPAG_BuffEffect::SetRemoveGameplayEffectTags(FGameplayTagContainer PassiveAbilityTagContainer)
{
	InheritableOwnedTagsContainer.CombinedTags = PassiveAbilityTagContainer;
}