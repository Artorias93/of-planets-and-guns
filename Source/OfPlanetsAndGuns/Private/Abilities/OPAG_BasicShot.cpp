#include "Abilities/OPAG_BasicShot.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/OPAG_BasicHitDamage.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"

UOPAG_BasicShot::UOPAG_BasicShot(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UOPAG_BasicShot::EventReceived(FGameplayEventData Payload)
{
	//This method works, but it's heavy. We keep it only for backup
	/*AActor* TargetActor = nullptr;
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), Payload.Target->GetClass(), Actors);

	for (auto Actor : Actors)
	{
		if (Actor->GetUniqueID() == Payload.Target->GetUniqueID())
		{
			TargetActor = Actor;
			break;
		}
	}

	if (TargetActor == nullptr )	
		return;*/

	FGameplayAbilityTargetDataHandle TargetDataHandle = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActor(GetOwningActorFromActorInfo());
	ApplyGameplayEffectToTarget(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), TargetDataHandle, UOPAG_BasicHitDamage::StaticClass(), 1, 1);
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);	
}

void UOPAG_BasicShot::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                      const FGameplayAbilityActorInfo* ActorInfo, 
								      const FGameplayAbilityActivationInfo ActivationInfo,
                                      const FGameplayEventData* TriggerEventData)
{
	const bool bResult = CommitAbility(Handle, ActorInfo, ActivationInfo);
	
	const FGameplayTag HitTag = FGameplayTag::RequestGameplayTag(TEXT("char.ability.ranged.hitEvent"));
	UAbilityTask_WaitGameplayEvent* WaitEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, HitTag, nullptr, false, true);
	WaitEvent->Activate();
	WaitEvent->EventReceived.AddDynamic(this, &UOPAG_BasicShot::EventReceived);
}
