#include "Abilities/OPAG_DashCooldown.h"

UOPAG_DashCooldown::UOPAG_DashCooldown()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;
	FScalableFloat DashScalableFloat;
	DashScalableFloat.SetValue(1.f);
	FGameplayEffectModifierMagnitude DashMagnitude(DashScalableFloat);
	DurationMagnitude = DashMagnitude;
	FInheritedTagContainer DashGrantedTags;
	FGameplayTagContainer DashGrantedTagsContainer;
	const FGameplayTag DashCooldownTag = FGameplayTag::RequestGameplayTag(TEXT("char.ability.movement.dash.cooldown"));
	DashGrantedTagsContainer.AddTag(DashCooldownTag);
	DashGrantedTags.CombinedTags = DashGrantedTagsContainer;
	DashGrantedTags.Added = DashGrantedTagsContainer;
	InheritableOwnedTagsContainer = DashGrantedTags;
}
