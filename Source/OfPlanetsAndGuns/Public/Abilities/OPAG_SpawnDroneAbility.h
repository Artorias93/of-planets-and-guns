#pragma once

#include "CoreMinimal.h"
#include "Abilities/OPAG_GameplayAbilityBase.h"
#include "OPAG_SpawnDroneAbility.generated.h"

UCLASS()
class OFPLANETSANDGUNS_API UOPAG_SpawnDroneAbility : public UOPAG_GameplayAbilityBase
{
	GENERATED_BODY()
	
public:
	UOPAG_SpawnDroneAbility(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo) const override;
};
