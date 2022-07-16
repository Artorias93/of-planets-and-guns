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

private:

	UFUNCTION()
	void LoseTarget();

public:

	AOPAG_AICBase();

	virtual void OnPossess(APawn* ControlledPawn) override;

	UFUNCTION()
	void OnPerceptionUpdated(AActor* UpdatedActor, FAIStimulus Stimulus);

	/////////////////////////////
	//Variables
	/////////////////////////////

	UPROPERTY(transient)
	class UBehaviorTreeComponent* BehaviorTreeComponent;

	UPROPERTY(transient)
	UBlackboardComponent* BlackboardComponent;

	FTimerHandle LoseSightTimerHandle;

protected:

	UPROPERTY(transient)
	UAIPerceptionComponent* PerceptionComp;

	UPROPERTY(transient)
	class UAISenseConfig_Sight* PerceptionSightConfig;

	UPROPERTY(transient)
	class UAISenseConfig_Damage* PerceptionDamageConfig;

public:

	UPROPERTY(EditAnywhere, Category = "AI")
	float LoseSightTimer = 3.f;

	UPROPERTY(EditAnywhere, Category = "AI")
	float SightSenseRadius = 2500.f;

	UPROPERTY(EditAnywhere, Category = "AI")
	float LoseSightSenseRadius = 3000.f;

	UPROPERTY(EditAnywhere, Category = "AI")
	float SightPeripheralVisionDegrees = 120.f;
};
