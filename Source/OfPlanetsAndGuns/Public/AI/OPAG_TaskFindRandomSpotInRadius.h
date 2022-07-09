#pragma once

#include "CoreMinimal.h"
#include "OPAG_TaskBase.h"
#include "OPAG_TaskFindRandomSpotInRadius.generated.h"

UCLASS()
class OFPLANETSANDGUNS_API UOPAG_TaskFindRandomSpotInRadius : public UOPAG_TaskBase
{
	GENERATED_BODY()
	
	/////////////////////////////
	//Functions
	/////////////////////////////

public:

	UOPAG_TaskFindRandomSpotInRadius(FObjectInitializer const& ObjectInitializer);

private:

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	/////////////////////////////
	//Variables
	/////////////////////////////

public:

	UPROPERTY(EditAnywhere, Category = "Blackboard Keys")
	FBlackboardKeySelector BBKeyPatrolLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Values")
	float PatrolRadius;
};