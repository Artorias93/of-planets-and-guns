#pragma once

#include "CoreMinimal.h"
#include "AI/OPAG_TaskBase.h"
#include "OPAG_TaskChangeChargeState.generated.h"

UCLASS()
class OFPLANETSANDGUNS_API UOPAG_TaskChangeChargeState : public UOPAG_TaskBase
{
	GENERATED_BODY()

	/////////////////////////////
	//Functions
	/////////////////////////////

public:

	UOPAG_TaskChangeChargeState(FObjectInitializer const& ObjectInitializer);

private:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerBTComponent, uint8* NodeMemory) override;

	/////////////////////////////
	//Variables
	/////////////////////////////
	
public:

	UPROPERTY(EditAnywhere, Category = "Values", meta=(AllowPrivateAccess = true))
	bool bIsCharging;

	
};
