// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/OPAG_HealthPotion.h"
#include "Abilities/OPAG_HealthPotionEffect.h"
#include "Abilities/OPAG_HealthPotionCooldown.h"
#include <GameplayEffectTypes.h>

UOPAG_HealthPotion::UOPAG_HealthPotion(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	CooldownGameplayEffectClass = UOPAG_HealthPotionCooldown::StaticClass();
	const FGameplayTag PotionTag = FGameplayTag::RequestGameplayTag(TEXT("char.ability.items.potion"));
	AbilityTags.AddTag(PotionTag);
}

void UOPAG_HealthPotion::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		const bool bResult = CommitAbility(Handle, ActorInfo, ActivationInfo);
		if (!bResult)
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		}

		UOPAG_HealthPotionEffect* Effect = NewObject<UOPAG_HealthPotionEffect>(UOPAG_HealthPotionEffect::StaticClass());

		FGameplayEffectSpec* GeSpec = new FGameplayEffectSpec(Effect, {}, 0.f);
		ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, FGameplayEffectSpecHandle(GeSpec));
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);

}

