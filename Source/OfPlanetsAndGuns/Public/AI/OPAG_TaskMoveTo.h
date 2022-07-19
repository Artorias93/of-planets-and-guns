#pragma once

#include "CoreMinimal.h"
#include "OPAG_TaskBase.h"
#include "OPAG_TaskMoveTo.generated.h"

//---------------------------------------------------------------------
//THIS TASK IS CURRENTLY UNUSED
//---------------------------------------------------------------------

UCLASS()
class OFPLANETSANDGUNS_API UOPAG_TaskMoveTo : public UOPAG_TaskBase
{
	GENERATED_BODY()

	/////////////////////////////
	//Functions
	/////////////////////////////

public:
	
	UOPAG_TaskMoveTo(FObjectInitializer const& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerBTComponent, uint8* NodeMemory) override;

	/////////////////////////////
	//Variables
	/////////////////////////////

	UPROPERTY(EditAnywhere, Category = "Blackboard Keys")
	FBlackboardKeySelector BBKeyDestination;

	UPROPERTY(EditAnywhere, Category = "AI")
	float AcceptedRadius = 90.f;
};
