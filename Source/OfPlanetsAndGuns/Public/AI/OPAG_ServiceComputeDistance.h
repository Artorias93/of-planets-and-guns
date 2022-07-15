#pragma once

#include "CoreMinimal.h"
#include "OPAG_ServiceBase.h"
#include "OPAG_ServiceComputeDistance.generated.h"

//---------------------------------------------------------------------
//THIS TASK IS CURRENTLY UNUSED
//---------------------------------------------------------------------

UCLASS()
class OFPLANETSANDGUNS_API UOPAG_ServiceComputeDistance : public UOPAG_ServiceBase
{
	GENERATED_BODY()

	/////////////////////////////
	//Functions
	/////////////////////////////

public:
	UOPAG_ServiceComputeDistance(FObjectInitializer const& ObjectInitializer);

	void TickNode(UBehaviorTreeComponent& OwnerBTComponent, uint8* NodeMemory, float DeltaSeconds) override;

	/////////////////////////////
	//Variables
	/////////////////////////////

public:

	UPROPERTY(EditAnywhere, Category = "Blackboard Keys")
	FBlackboardKeySelector BBKeyTargetActor;

	UPROPERTY(EditAnywhere, Category = "Blackboard Keys")
	FBlackboardKeySelector BBKeyEnemyDistance;
};
