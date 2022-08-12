#pragma once

#include "CoreMinimal.h"
#include "Abilities/OPAG_GameplayAbilityBase.h"
#include "OPAG_HitShieldAbility.generated.h"

UCLASS()
class OFPLANETSANDGUNS_API UOPAG_HitShieldAbility : public UOPAG_GameplayAbilityBase
{
	GENERATED_BODY()

public:
	UOPAG_HitShieldAbility(const FObjectInitializer& ObjectInitializer);

protected:
	UFUNCTION() void EventReceived(FGameplayEventData Payload);
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

};
