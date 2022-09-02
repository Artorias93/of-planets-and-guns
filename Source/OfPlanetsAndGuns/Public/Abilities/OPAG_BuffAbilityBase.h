#pragma once

#include "CoreMinimal.h"
#include "Abilities/OPAG_GameplayAbilityBase.h"
#include <GameplayEffect.h>
#include "OPAG_BuffAbilityBase.generated.h"

class UOPAG_BuffEffect;

UCLASS()
class OFPLANETSANDGUNS_API UOPAG_BuffAbilityBase : public UOPAG_GameplayAbilityBase
{
	GENERATED_BODY()

public:
	UOPAG_BuffAbilityBase(const FObjectInitializer& ObjectInitializer);

protected:
	UPROPERTY(Category = "OPAG | AbilitySystem | Effect", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FGameplayModifierInfo ModifierInfo;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

};
