// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/OPAG_TaskWaitControllerIsLost.h"
#include "Characters/Enemies/OPAG_AICBase.h"

UOPAG_TaskWaitControllerIsLost::UOPAG_TaskWaitControllerIsLost(const FObjectInitializer& objectInitializer)
	: Super(objectInitializer)
{
	bNotifyTick = true;
}

void UOPAG_TaskWaitControllerIsLost::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (AOPAG_AICBase* const AIController = Cast<AOPAG_AICBase>(OwnerComp.GetAIOwner()))
	{
		// should never occur
		if (AIController == nullptr)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
}

EBTNodeResult::Type UOPAG_TaskWaitControllerIsLost::ExecuteTask(UBehaviorTreeComponent& OwnerBTComponent, uint8* NodeMemory)
{
	return EBTNodeResult::InProgress;
}