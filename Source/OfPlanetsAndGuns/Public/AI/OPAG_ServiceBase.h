#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "OPAG_ServiceBase.generated.h"

UCLASS()
class OFPLANETSANDGUNS_API UOPAG_ServiceBase : public UBTService
{
	GENERATED_BODY()

	/////////////////////////////
	//Functions
	/////////////////////////////

private:

	virtual FString GetStaticDescription() const override;	
};
