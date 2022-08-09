#include "Abilities/OPAG_ShieldAbilityCooldown.h"

UOPAG_ShieldAbilityCooldown::UOPAG_ShieldAbilityCooldown()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;
	FScalableFloat ShieldScalableFloat;
	ShieldScalableFloat.SetValue(5.f);
	FGameplayEffectModifierMagnitude ShieldMagnitude(ShieldScalableFloat);
	DurationMagnitude = ShieldMagnitude;
	FInheritedTagContainer ShieldGrantedTags;
	FGameplayTagContainer ShieldGrantedTagsContainer;
	const FGameplayTag ShieldCooldownTag = FGameplayTag::RequestGameplayTag(TEXT("char.ability.shield.cooldown"));
	ShieldGrantedTagsContainer.AddTag(ShieldCooldownTag);
	ShieldGrantedTags.CombinedTags = ShieldGrantedTagsContainer;
	ShieldGrantedTags.Added = ShieldGrantedTagsContainer;
	InheritableOwnedTagsContainer = ShieldGrantedTags;
}
