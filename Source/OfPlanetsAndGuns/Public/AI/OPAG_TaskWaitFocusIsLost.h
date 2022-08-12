// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/OPAG_TaskBase.h"
#include "OPAG_TaskWaitFocusIsLost.generated.h"

class AOPAG_AICBase;

/*
*	The following task is a long-running type, and its purpose is just to use as a placeholder in order to have at least a node in a BT
*	It simply continue to run until the AI lose the focus actor
*/

UCLASS()
class OFPLANETSANDGUNS_API UOPAG_TaskWaitFocusIsLost : public UOPAG_TaskBase
{
	GENERATED_BODY()

protected:
	UOPAG_TaskWaitFocusIsLost(const FObjectInitializer& objectInitializer);
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerBTComponent, uint8* NodeMemory) override;
};
