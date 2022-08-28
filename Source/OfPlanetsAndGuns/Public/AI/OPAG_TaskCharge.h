#pragma once

#include "CoreMinimal.h"
#include "OPAG_TaskBase.h"
#include "BehaviorTree/BTTaskNode.h"
#include "OPAG_TaskCharge.generated.h"

UCLASS()
class OFPLANETSANDGUNS_API UOPAG_TaskCharge : public UOPAG_TaskBase
{
	GENERATED_BODY()

	/////////////////////////////
	//Functions
	/////////////////////////////

public:

	UOPAG_TaskCharge(FObjectInitializer const& ObjectInitializer);

private:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerBTComponent, uint8* NodeMemory) override;

	/////////////////////////////
	//Variables
	/////////////////////////////

private:

	UPROPERTY(EditAnywhere, Category = "Blackboard Keys", meta=(AllowPrivateAccess = true))
	FBlackboardKeySelector BBKeyTargetLocation;

	UPROPERTY(EditAnywhere, Category = "Values", meta=(AllowPrivateAccess = true))
	float ChargeDistanceBehindPlayer = 300.f;

	UPROPERTY(EditAnywhere, Category = "Values", meta=(AllowPrivateAccess = true))
	FVector BoxTraceHalfSize = FVector(50.f, 50.f, 50.f);

	UPROPERTY(EditAnywhere, Category = "Values", meta=(AllowPrivateAccess = true))
	FVector QueryingExtent = FVector(50.f, 50.f, 150.f);
	
};
