#pragma once

#include "CoreMinimal.h"
#include "Abilities/OPAG_GameplayAbilityBase.h"
#include "OPAG_HealthBuffAbility.generated.h"

UCLASS()
class OFPLANETSANDGUNS_API UOPAG_HealthBuffAbility : public UOPAG_GameplayAbilityBase
{
	GENERATED_BODY()

public:
	UOPAG_HealthBuffAbility(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

};