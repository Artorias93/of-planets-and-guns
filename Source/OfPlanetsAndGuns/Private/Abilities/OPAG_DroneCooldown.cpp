#include "Abilities/OPAG_DroneCooldown.h"

UOPAG_DroneCooldown::UOPAG_DroneCooldown()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;
	FScalableFloat DroneScalableFloat;
	DroneScalableFloat.SetValue(.0f);
	FGameplayEffectModifierMagnitude DroneMagnitude(DroneScalableFloat);
	DurationMagnitude = DroneMagnitude;
	FInheritedTagContainer DroneGrantedTags;
	FGameplayTagContainer DroneGrantedTagsContainer;
	const FGameplayTag DroneCooldownTag = FGameplayTag::RequestGameplayTag(TEXT("char.ability.drone.cooldown"));
	DroneGrantedTagsContainer.AddTag(DroneCooldownTag);
	DroneGrantedTags.CombinedTags = DroneGrantedTagsContainer;
	DroneGrantedTags.Added = DroneGrantedTagsContainer;
	InheritableOwnedTagsContainer = DroneGrantedTags;
}