#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include <GameplayEffect.h>
#include "OPAG_BuffEffect.generated.h"

UCLASS()
class OFPLANETSANDGUNS_API UOPAG_BuffEffect : public UGameplayEffect
{
	GENERATED_BODY()
	
public:
	UOPAG_BuffEffect();

public:
	void SetModifierInfo(FGameplayModifierInfo ModifierInfo);
	void SetRemoveGameplayEffectTags(FGameplayTagContainer PassiveAbilityTagContainer);
};
