#include "Abilities/OPAG_HitShieldAbility.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/OPAG_HitPlayerShieldEffect.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"

UOPAG_HitShieldAbility::UOPAG_HitShieldAbility(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

void UOPAG_HitShieldAbility::EventReceived(FGameplayEventData Payload)
{
	FGameplayAbilityTargetDataHandle TargetDataHandle = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActor(GetOwningActorFromActorInfo());
	
	ApplyGameplayEffectToTarget(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), TargetDataHandle, UOPAG_HitPlayerShieldEffect::StaticClass(), 1, 1);
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
}

void UOPAG_HitShieldAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	const bool bResult = CommitAbility(Handle, ActorInfo, ActivationInfo);
	const FGameplayTag HitTag = FGameplayTag::RequestGameplayTag(TEXT("char.ability.shield.hitEvent"));

	UAbilityTask_WaitGameplayEvent* WaitEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, HitTag, nullptr, false, true);
	WaitEvent->Activate();
	WaitEvent->EventReceived.AddDynamic(this, &UOPAG_HitShieldAbility::EventReceived);
}