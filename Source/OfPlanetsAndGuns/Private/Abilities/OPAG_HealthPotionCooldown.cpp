#include "Abilities/OPAG_HealthPotionCooldown.h"

UOPAG_HealthPotionCooldown::UOPAG_HealthPotionCooldown()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;
	FScalableFloat PotionScalableFloat;
	PotionScalableFloat.SetValue(.5f);
	FGameplayEffectModifierMagnitude PotionMagnitude(PotionScalableFloat);
	DurationMagnitude = PotionMagnitude;
	FInheritedTagContainer PotionGrantedTags;
	FGameplayTagContainer PotionGrantedTagsContainer;
	const FGameplayTag PotionCooldownTag = FGameplayTag::RequestGameplayTag(TEXT("char.ability.items.potion.cooldown"));
	PotionGrantedTagsContainer.AddTag(PotionCooldownTag);
	PotionGrantedTags.CombinedTags = PotionGrantedTagsContainer;
	PotionGrantedTags.Added = PotionGrantedTagsContainer;
	InheritableOwnedTagsContainer = PotionGrantedTags;
}
