#pragma once

#include "AbilitySystemComponent.h"
#include "Characters/Player/OPAG_AttributeSetPlayer.h"

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "OPAG_DamageShieldCalculation.generated.h"

struct FOPAG_DamageShieldAttributes
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(ShieldCapacity);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageTakenAmount);

	FOPAG_DamageShieldAttributes()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UOPAG_AttributeSetPlayer, ShieldCapacity, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UOPAG_AttributeSetPlayer, DamageTakenAmount, Source, true);
	}
};

UCLASS()
class OFPLANETSANDGUNS_API UOPAG_DamageShieldCalculation : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UOPAG_DamageShieldCalculation(const FObjectInitializer& ObjectInitializer);
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

};
