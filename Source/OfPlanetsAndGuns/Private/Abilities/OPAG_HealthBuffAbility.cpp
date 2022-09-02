#include "Abilities/OPAG_HealthBuffAbility.h"
#include "Abilities/OPAG_HealthBuffEffect.h"

UOPAG_HealthBuffAbility::UOPAG_HealthBuffAbility(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	const FGameplayTag HeathBuffTag = FGameplayTag::RequestGameplayTag(TEXT("char.ability.buff.health"));
	AbilityTags.AddTag(HeathBuffTag);
}

void UOPAG_HealthBuffAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		const bool bResult = CommitAbility(Handle, ActorInfo, ActivationInfo);
		if (!bResult)
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		}

		UOPAG_HealthBuffEffect* Effect = NewObject<UOPAG_HealthBuffEffect>(UOPAG_HealthBuffEffect::StaticClass());

		FGameplayEffectSpec* GeSpec = new FGameplayEffectSpec(Effect, {}, 0.f);
		ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, FGameplayEffectSpecHandle(GeSpec));
	}
	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}