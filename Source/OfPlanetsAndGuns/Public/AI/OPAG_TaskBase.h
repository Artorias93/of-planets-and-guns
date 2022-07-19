#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "OPAG_TaskBase.generated.h"

UCLASS()
class OFPLANETSANDGUNS_API UOPAG_TaskBase : public UBTTaskNode
{
	GENERATED_BODY()

	/////////////////////////////
	//Functions
	/////////////////////////////

private:
	
	virtual FString GetStaticDescription() const override;	
};
