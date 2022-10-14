#include "Abilities/OPAG_DamageShieldCalculation.h"

UOPAG_DamageShieldCalculation::UOPAG_DamageShieldCalculation(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	const FOPAG_DamageShieldAttributes DamageShieldStruct;

	RelevantAttributesToCapture.Add(DamageShieldStruct.ShieldCapacityDef);
	#if WITH_EDITORONLY_DATA
	InvalidScopedModifierAttributes.Add(DamageShieldStruct.ShieldCapacityDef);
	#endif
	RelevantAttributesToCapture.Add(DamageShieldStruct.DamageTakenAmountDef);
}

void UOPAG_DamageShieldCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FOPAG_DamageShieldAttributes DamageShieldStruct;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const UAbilitySystemComponent* const Target = ExecutionParams.GetTargetAbilitySystemComponent();
	const UAbilitySystemComponent* const Source = ExecutionParams.GetSourceAbilitySystemComponent();

	//TODO: Unused local variables?
	AActor* TargetActor = Target ? Target->GetAvatarActor() : nullptr;
	AActor* SourceActor = Source ? Source->GetAvatarActor() : nullptr;

	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float ShieldCapacity = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageShieldStruct.ShieldCapacityDef, EvaluationParameters, ShieldCapacity);

	float DamageTaken = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageShieldStruct.DamageTakenAmountDef, EvaluationParameters, DamageTaken);

	//Replace 1 with an optional attack multiplier
	const float EffectiveDamageTaken = 1 * DamageTaken;

	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageShieldStruct.ShieldCapacityProperty, EGameplayModOp::Additive, -EffectiveDamageTaken));
}