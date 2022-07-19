#pragma once

#include "CoreMinimal.h"
#include "OPAG_TaskBase.h"
#include "OPAG_TaskSetNextIndexPatrolPath.generated.h"

UCLASS()
class OFPLANETSANDGUNS_API UOPAG_TaskSetNextIndexPatrolPath : public UOPAG_TaskBase
{
	GENERATED_BODY()

	/////////////////////////////
	//Functions
	/////////////////////////////

public:

	UOPAG_TaskSetNextIndexPatrolPath(FObjectInitializer const& ObjectInitializer);

private:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerBTComponent, uint8* NodeMemory) override;

	/////////////////////////////
	//Variables
	/////////////////////////////

private:

	UPROPERTY(EditAnywhere, Category = "Blackboard Keys", meta=(AllowPrivateAccess = true))
	FBlackboardKeySelector BBKeyPatrolPathIndex;

	UPROPERTY(EditAnywhere, Category = "Blackboard Keys", meta=(AllowPrivateAccess = true))
	FBlackboardKeySelector BBKeyPatrolLocation;	
};
