#pragma once

#include "CoreMinimal.h"
#include "OPAG_TaskBase.h"
#include "BehaviorTree/BTTaskNode.h"
#include "OPAG_TaskMoveDirectlyToward.generated.h"

UCLASS()
class OFPLANETSANDGUNS_API UOPAG_TaskMoveDirectlyToward : public UOPAG_TaskBase
{
	GENERATED_BODY()

	float AlphaLerp;

	float Distance;

	FVector ChargeLoc;

	FVector AgentLocation;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerBTComponent, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;


public:

	UPROPERTY(EditAnywhere, Category = "Values", meta=(AllowPrivateAccess = true))
	float MoveSpeed = 20.f;
	
	UOPAG_TaskMoveDirectlyToward(FObjectInitializer const& ObjectInitializer);

	UPROPERTY(EditAnywhere, Category = "Blackboard Keys", meta=(AllowPrivateAccess = true))
	FBlackboardKeySelector BBKeyChargeLocation;
	
};
