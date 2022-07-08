#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "OPAG_MeleeEnemyAIController.generated.h"

UCLASS()
class OFPLANETSANDGUNS_API AOPAG_MeleeEnemyAIController : public AAIController
{
	GENERATED_BODY()

	//Why transient?
	UPROPERTY(transient)
	class UBehaviorTreeComponent* BehaviorTreeComponent;

	//Why transient?
	UPROPERTY(transient)
	class UBlackboardComponent* BlackboardComponent;
	
public:
	AOPAG_MeleeEnemyAIController();

	void OnPossess(APawn* ControlledPawn);
};
