#pragma once

#include "CoreMinimal.h"
#include "Abilities/OPAG_GameplayAbilityBase.h"
#include "OPAG_HealthPotion.generated.h"

class UOPAG_HealthPotionEffect;

UCLASS()
class OFPLANETSANDGUNS_API UOPAG_HealthPotion : public UOPAG_GameplayAbilityBase
{
	GENERATED_BODY()
public: 
	UOPAG_HealthPotion(const FObjectInitializer& ObjectInitializer);

private:
	UPROPERTY(Category = "OPAG | Ability | Potion", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UOPAG_HealthPotionEffect> PotionEffectClass;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
