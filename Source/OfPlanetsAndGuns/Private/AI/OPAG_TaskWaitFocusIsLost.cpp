// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/OPAG_TaskWaitFocusIsLost.h"
#include "Characters/Enemies/OPAG_AICBase.h"

UOPAG_TaskWaitFocusIsLost::UOPAG_TaskWaitFocusIsLost(const FObjectInitializer& objectInitializer)
	: Super(objectInitializer)
{
	bNotifyTick = true;
}

void UOPAG_TaskWaitFocusIsLost::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (AOPAG_AICBase* const AIController = Cast<AOPAG_AICBase>(OwnerComp.GetAIOwner()))
	{
		if (AIController->GetFocusActor() == nullptr)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
}

EBTNodeResult::Type UOPAG_TaskWaitFocusIsLost::ExecuteTask(UBehaviorTreeComponent& OwnerBTComponent, uint8* NodeMemory)
{
	return EBTNodeResult::InProgress;
}
