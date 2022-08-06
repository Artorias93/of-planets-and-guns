#pragma once

#include "CoreMinimal.h"
#include "OPAG_TaskBase.h"
#include "BehaviorTree/BTTaskNode.h"
#include "OPAG_TaskCharge.generated.h"

UCLASS()
class OFPLANETSANDGUNS_API UOPAG_TaskCharge : public UOPAG_TaskBase
{
	GENERATED_BODY()

public:

	UOPAG_TaskCharge(FObjectInitializer const& ObjectInitializer);

private:

	UPROPERTY(EditAnywhere, Category = "Blackboard Keys", meta=(AllowPrivateAccess = true))
	FBlackboardKeySelector BBKeyTargetLocation;

	UPROPERTY(EditAnywhere, Category = "Values", meta=(AllowPrivateAccess = true))
	float ChargeDistance = 20.f;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerBTComponent, uint8* NodeMemory) override;
	
};
