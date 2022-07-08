#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "OPAG_TaskFindRandomSpotInRadius.generated.h"

UCLASS()
class OFPLANETSANDGUNS_API UOPAG_TaskFindRandomSpotInRadius : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
	//Variables

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float PatrolRadius;

	//Functions

public:

	UOPAG_TaskFindRandomSpotInRadius(FObjectInitializer const& ObjectInitializer);

private:

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
