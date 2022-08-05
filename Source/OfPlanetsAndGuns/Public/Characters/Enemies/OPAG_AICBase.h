#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h" //To use FAIStimulus in the function signature
#include "OPAG_AICBase.generated.h"

UCLASS()
class OFPLANETSANDGUNS_API AOPAG_AICBase : public AAIController
{
	GENERATED_BODY()

	/////////////////////////////
	//Functions
	/////////////////////////////

public:

	AOPAG_AICBase();

protected:

	virtual void OnPossess(APawn* ControlledPawn) override;

	UFUNCTION()
	void OnPerceptionUpdated(AActor* UpdatedActor, FAIStimulus Stimulus);

	UFUNCTION()
	void LoseTarget();

private:

	/////////////////////////////
	//Variables
	/////////////////////////////

protected:

	UPROPERTY(EditAnywhere, Category = "OPAG AI", meta=(AllowPrivateAccess = true))
	float LoseSightTimer = 3.f;

	UPROPERTY(transient)
	class UBehaviorTreeComponent* BehaviorTreeComponent;

	UPROPERTY(transient)
	UBlackboardComponent* BlackboardComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UAIPerceptionComponent* PerceptionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UAISenseConfig_Sight* PerceptionSightConfig;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UAISenseConfig_Damage* PerceptionDamageConfig;	
	
	FTimerHandle LoseSightTimerHandle;
};
