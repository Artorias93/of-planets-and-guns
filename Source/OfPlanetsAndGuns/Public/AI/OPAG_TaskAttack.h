#pragma once

#include "CoreMinimal.h"
#include "OPAG_TaskBase.h"
#include "OPAG_TaskAttack.generated.h"

UCLASS()
class OFPLANETSANDGUNS_API UOPAG_TaskAttack : public UOPAG_TaskBase
{
	GENERATED_BODY()

	/////////////////////////////
	//Functions
	/////////////////////////////

public:

	UOPAG_TaskAttack(FObjectInitializer const& ObjectInitializer);

private:
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerBTComponent, uint8* NodeMemory) override;		
};
