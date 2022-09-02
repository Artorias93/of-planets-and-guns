#include "Abilities/OPAG_BuffAbilityBase.h"
#include "Abilities/OPAG_BuffEffect.h"

UOPAG_BuffAbilityBase::UOPAG_BuffAbilityBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UOPAG_BuffAbilityBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		const bool bResult = CommitAbility(Handle, ActorInfo, ActivationInfo);
		if (!bResult)
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		}

		UOPAG_BuffEffect* Effect = NewObject<UOPAG_BuffEffect>(UOPAG_BuffEffect::StaticClass());
		Effect->SetModifierInfo(ModifierInfo);
		Effect->SetRemoveGameplayEffectTags(AbilityTags);

		FGameplayEffectSpec* GeSpec = new FGameplayEffectSpec(Effect, {}, 0.f);
		ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, FGameplayEffectSpecHandle(GeSpec));
	}
	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}