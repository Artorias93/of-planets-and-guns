#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "OPAG_TaskSetNextIndexPatrolPath.generated.h"

UCLASS()
class OFPLANETSANDGUNS_API UOPAG_TaskSetNextIndexPatrolPath : public UBTTaskNode
{
	GENERATED_BODY()

	/////////////////////////////
	//Functions
	/////////////////////////////

public:

	UOPAG_TaskSetNextIndexPatrolPath(FObjectInitializer const& ObjectInitializer);

private:

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerBTComponent, uint8* NodeMemory);

	/////////////////////////////
	//Variables
	/////////////////////////////

public:

	UPROPERTY(EditAnywhere, Category = "Blackboard Keys")
	FBlackboardKeySelector BBKeyPatrolPathIndex;

	UPROPERTY(EditAnywhere, Category = "Blackboard Keys")
	FBlackboardKeySelector BBKeyPatrolLocation;	
};
