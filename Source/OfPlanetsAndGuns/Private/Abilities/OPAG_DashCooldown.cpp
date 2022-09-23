#include "Abilities/OPAG_DashCooldown.h"
#include "Characters/Player/OPAG_AttributeSetPlayer.h"

UOPAG_DashCooldown::UOPAG_DashCooldown()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;

	// Initialize restoring time to player's attribute
	FAttributeBasedFloat DashAttributeBasedFloat;
	FGameplayEffectAttributeCaptureDefinition DashAttributeCaptureDefinition(UOPAG_AttributeSetPlayer::GetDashRestoreTimeAttribute(), 
		EGameplayEffectAttributeCaptureSource::Source, false);
	DashAttributeBasedFloat.BackingAttribute = DashAttributeCaptureDefinition;

	// Assign DashAttributeBasedFloat to duration
	FGameplayEffectModifierMagnitude DashMagnitude(DashAttributeBasedFloat);
	DurationMagnitude = DashMagnitude;

	// Initialize dash tag
	FInheritedTagContainer DashGrantedTags;
	FGameplayTagContainer DashGrantedTagsContainer;
	const FGameplayTag DashCooldownTag = FGameplayTag::RequestGameplayTag(TEXT("char.ability.movement.dash.cooldown"));
	DashGrantedTagsContainer.AddTag(DashCooldownTag);
	DashGrantedTags.CombinedTags = DashGrantedTagsContainer;
	DashGrantedTags.Added = DashGrantedTagsContainer;
	InheritableOwnedTagsContainer = DashGrantedTags;
}
