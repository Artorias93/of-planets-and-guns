#include "Abilities/OPAG_HitPlayerShieldEffect.h"
#include "Abilities/OPAG_DamageShieldCalculation.h"

UOPAG_HitPlayerShieldEffect::UOPAG_HitPlayerShieldEffect()
{
	TArray<FGameplayEffectExecutionDefinition> ExecutionDefition;
	FGameplayEffectExecutionDefinition DamageExecution;

	DamageExecution.CalculationClass = UOPAG_DamageShieldCalculation::StaticClass();
	ExecutionDefition.Add(DamageExecution);
	Executions = ExecutionDefition;
}