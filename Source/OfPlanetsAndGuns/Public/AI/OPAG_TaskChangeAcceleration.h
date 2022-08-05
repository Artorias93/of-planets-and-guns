#pragma once

#include "CoreMinimal.h"
#include "OPAG_TaskBase.h"
#include "OPAG_TaskChangeAcceleration.generated.h"

UCLASS()
class OFPLANETSANDGUNS_API UOPAG_TaskChangeAcceleration : public UOPAG_TaskBase
{
	GENERATED_BODY()
	/////////////////////////////
	//Functions
	/////////////////////////////

	public:

	UOPAG_TaskChangeAcceleration(FObjectInitializer const& ObjectInitializer);

private:
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerBTComponent, uint8* NodeMemory) override;

	/////////////////////////////
	//Variables
	/////////////////////////////

	private:

	UPROPERTY(EditAnywhere, Category = "Values", meta = (AllowPrivateAccess = true))
	float NewAcceleration = 2048.f;
};
