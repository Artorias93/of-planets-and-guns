#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "OPAG_TaskChangeSpeed.generated.h"

UCLASS()
class OFPLANETSANDGUNS_API UOPAG_TaskChangeSpeed : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	//Variables

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float NewSpeed = 600.f;

	//Functions

public:

	UOPAG_TaskChangeSpeed(FObjectInitializer const& ObjectInitializer);	

private:

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerBTComponent, uint8* NodeMemory);
};
