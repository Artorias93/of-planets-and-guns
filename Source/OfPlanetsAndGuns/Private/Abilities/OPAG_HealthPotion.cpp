// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/OPAG_HealthPotion.h"
#include "Abilities/OPAG_HealthPotionEffect.h"
#include "Abilities/OPAG_HealthPotionCooldown.h"

UOPAG_HealthPotion::UOPAG_HealthPotion(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	CooldownGameplayEffectClass = UOPAG_HealthPotionCooldown::StaticClass();
	const FGameplayTag PotionTag = FGameplayTag::RequestGameplayTag(TEXT("char.ability.items.potion"));
	AbilityTags.AddTag(PotionTag);
}

void UOPAG_HealthPotion::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	const bool bResult = CommitAbility(Handle, ActorInfo, ActivationInfo);

	const UGameplayEffect* Effect = UOPAG_HealthPotionEffect::StaticClass()->GetDefaultObject<UGameplayEffect>();
	
	
	ApplyGameplayEffectToOwner(Handle,	ActorInfo,	ActivationInfo,	Effect,	1,	1);
}

void UOPAG_HealthPotion::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{

}
