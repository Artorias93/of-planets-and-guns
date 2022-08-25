#include "Abilities/OPAG_BasicHitDamage.h"
#include "DamageExecutionGAS.h"

UOPAG_BasicHitDamage::UOPAG_BasicHitDamage()
{
	TArray<FGameplayEffectExecutionDefinition> ExecutionDefition;

	FGameplayEffectExecutionDefinition DamageExecution;

	DamageExecution.CalculationClass = UDamageExecutionGAS::StaticClass();
	ExecutionDefition.Add(DamageExecution);

	Executions = ExecutionDefition;
}
