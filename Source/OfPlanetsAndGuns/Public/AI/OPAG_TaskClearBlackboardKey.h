#pragma once

#include "CoreMinimal.h"
#include "AI/OPAG_TaskBase.h"
#include "OPAG_TaskClearBlackboardKey.generated.h"

UCLASS()
class OFPLANETSANDGUNS_API UOPAG_TaskClearBlackboardKey : public UOPAG_TaskBase
{
	GENERATED_BODY()

	/////////////////////////////
	//Functions
	/////////////////////////////

public:

	UOPAG_TaskClearBlackboardKey(FObjectInitializer const& ObjectInitializer);

private:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerBTComponent, uint8* NodeMemory) override;	
	
	/////////////////////////////
	//Variables
	/////////////////////////////
	
private:

	UPROPERTY(EditAnywhere, Category = "Blackboard Keys", meta=(AllowPrivateAccess = true))
	FBlackboardKeySelector BBKeyToClear;	
};
