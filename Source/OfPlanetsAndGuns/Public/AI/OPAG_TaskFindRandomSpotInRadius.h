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

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerBTComponent, uint8* NodeMemory) override;

	/////////////////////////////
	//Variables
	/////////////////////////////

private:

	UPROPERTY(EditAnywhere, Category = "Blackboard Keys", meta=(AllowPrivateAccess = true))
	FBlackboardKeySelector BBKeyPatrolLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Values", meta=(AllowPrivateAccess = true))
	float PatrolRadius = 1000.f;
};