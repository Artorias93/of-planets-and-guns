// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/OPAG_TaskBase.h"
#include "OPAG_TaskWaitControllerIsLost.generated.h"

/**
 * 
 */
UCLASS()
class OFPLANETSANDGUNS_API UOPAG_TaskWaitControllerIsLost : public UOPAG_TaskBase
{
	GENERATED_BODY()

protected:
	UOPAG_TaskWaitControllerIsLost(const FObjectInitializer& objectInitializer);
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerBTComponent, uint8* NodeMemory) override;	
};
