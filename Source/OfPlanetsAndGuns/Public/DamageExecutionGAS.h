#pragma once

#include "AbilitySystemComponent.h"
#include "Characters/OPAG_AttributeSetBase.h"

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"

#include "DamageExecutionGAS.generated.h"

struct FDamageAttributes
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageMultiplier);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageTakenAmount);

	FDamageAttributes()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UOPAG_AttributeSetBase, Health, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UOPAG_AttributeSetBase, DamageMultiplier, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UOPAG_AttributeSetBase, DamageTakenAmount, Source, true);
	}
};

UCLASS()
class OFPLANETSANDGUNS_API UDamageExecutionGAS : public UGameplayEffectExecutionCalculation
{
	GENERATED_UCLASS_BODY()

public:
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
