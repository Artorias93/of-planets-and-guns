#pragma once

#include "CoreMinimal.h"
#include "Abilities/OPAG_GameplayAbilityBase.h"
#include "OPAG_ShieldAbility.generated.h"

UCLASS()
class OFPLANETSANDGUNS_API UOPAG_ShieldAbility : public UOPAG_GameplayAbilityBase
{
	GENERATED_BODY()

public:
	UOPAG_ShieldAbility(const FObjectInitializer& ObjectInitializer);

private:
	TSubclassOf<AActor> ShieldClass;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

};
