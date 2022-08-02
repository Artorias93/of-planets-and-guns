// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/OPAG_HealthPotion.h"
#include "Abilities/OPAG_HealthPotionEffect.h"

void UOPAG_HealthPotion::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	const bool bResult = CommitAbility(Handle, ActorInfo, ActivationInfo);

	const UGameplayEffect* Effect = UOPAG_HealthPotionEffect::StaticClass()->GetDefaultObject<UGameplayEffect>();
	
	ApplyGameplayEffectToOwner(
			GetCurrentAbilitySpecHandle(),
			GetCurrentActorInfo(),
			GetCurrentActivationInfo(),
			Effect,
			1,
			1);
}
