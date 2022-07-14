#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "OPAG_TaskMoveTo.generated.h"

UCLASS()
class OFPLANETSANDGUNS_API UOPAG_TaskMoveTo : public UBTTaskNode
{
	GENERATED_BODY()

	/////////////////////////////
	//Functions
	/////////////////////////////

public:
	
	UOPAG_TaskMoveTo(FObjectInitializer const& ObjectInitializer);

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerBTComponent, uint8* NodeMemory);

	/////////////////////////////
	//Variables
	/////////////////////////////

	UPROPERTY(EditAnywhere, Category = "Blackboard Keys")
	FBlackboardKeySelector BBKeyDestination;

	UPROPERTY(EditAnywhere, Category = "AI")
	float AcceptedRadius = 90.f;
};
