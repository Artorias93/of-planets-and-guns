#include "DamageExecutionGAS.h"

UDamageExecutionGAS::UDamageExecutionGAS(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	const FDamageAttributes DamageStruct;

	RelevantAttributesToCapture.Add(DamageStruct.HealthDef);
#if WITH_EDITORONLY_DATA
	InvalidScopedModifierAttributes.Add(DamageStruct.HealthDef);
#endif
	RelevantAttributesToCapture.Add(DamageStruct.DamageTakenAmountDef);
	RelevantAttributesToCapture.Add(DamageStruct.DamageMultiplierDef);
}

void UDamageExecutionGAS::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FDamageAttributes DamageStruct;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const UAbilitySystemComponent* const Target = ExecutionParams.GetTargetAbilitySystemComponent();
	const UAbilitySystemComponent* const Source = ExecutionParams.GetSourceAbilitySystemComponent();

	AActor* TargetActor = Target ? Target->AvatarActor : nullptr;
	AActor* SourceActor = Source ? Source->AvatarActor : nullptr;

	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags; 
	EvaluationParameters.TargetTags = TargetTags;

	float Health = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStruct.HealthDef, EvaluationParameters, Health);

	float DamageTaken = 0.f; 
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStruct.DamageTakenAmountDef, EvaluationParameters, DamageTaken);

	float DamageMultiplier = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStruct.DamageMultiplierDef, EvaluationParameters, DamageMultiplier);

	//Replace 1 with an optional attack multiplier
	const float EffectiveDamageTaken = DamageMultiplier * DamageTaken;

	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStruct.HealthProperty, EGameplayModOp::Additive, -EffectiveDamageTaken));
}