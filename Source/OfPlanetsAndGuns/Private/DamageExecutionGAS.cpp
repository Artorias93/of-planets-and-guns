// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageExecutionGAS.h"

UDamageExecutionGAS::UDamageExecutionGAS(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	FDamageAttributes DamageStruct;

	RelevantAttributesToCapture.Add(DamageStruct.HealthDef);
	InvalidScopedModifierAttributes.Add(DamageStruct.HealthDef);

	RelevantAttributesToCapture.Add(DamageStruct.DamagedDef);
}

void UDamageExecutionGAS::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	FDamageAttributes DamageStruct;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	UAbilitySystemComponent* Target = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* Source = ExecutionParams.GetSourceAbilitySystemComponent();

	AActor* TargetActor = Target ? Target->AvatarActor : nullptr;
	AActor* SourceActor = Source ? Source->AvatarActor : nullptr;

	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags; 
	EvaluationParameters.TargetTags = TargetTags;

	float Health = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStruct.HealthDef, EvaluationParameters, Health);

	float AttackMultiplier = 0.f; 
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStruct.DamagedDef, EvaluationParameters, AttackMultiplier);

	float DamageDone = 1 * AttackMultiplier;

	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStruct.HealthProperty, EGameplayModOp::Additive, -DamageDone));
}