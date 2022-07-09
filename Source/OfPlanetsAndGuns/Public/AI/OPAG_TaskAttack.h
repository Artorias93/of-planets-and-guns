#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "OPAG_TaskAttack.generated.h"

UCLASS()
class OFPLANETSANDGUNS_API UOPAG_TaskAttack : public UBTTaskNode
{
	GENERATED_BODY()

	/////////////////////////////
	//Functions
	/////////////////////////////

public:

	UOPAG_TaskAttack(FObjectInitializer const& ObjectInitializer);

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerBTComponent, uint8* NodeMemory);

	/////////////////////////////
	//Variables
	/////////////////////////////
	
		
};
