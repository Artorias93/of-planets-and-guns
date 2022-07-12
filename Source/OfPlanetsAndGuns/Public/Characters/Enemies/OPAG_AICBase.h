#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "OPAG_AICBase.generated.h"

UCLASS()
class OFPLANETSANDGUNS_API AOPAG_AICBase : public AAIController
{
	GENERATED_BODY()

	//Variables

	UPROPERTY(transient)
	class UBehaviorTreeComponent* BehaviorTreeComponent;

	UPROPERTY(transient)
	class UBlackboardComponent* BlackboardComponent;

	FTimerHandle LoseSightTimerHandle;

protected:

	UPROPERTY(transient)
	class UAIPerceptionComponent* PerceptionComp;

	UPROPERTY(transient)
	class UAISenseConfig_Sight* PerceptionSightConfig;

	UPROPERTY(transient)
	class UAISenseConfig_Damage* PerceptionDamageConfig;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float LoseSightTimer = 3.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float SightSenseRadius = 3000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float LoseSightSenseRadius = 3500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float SightPeripheralVisionDegrees = 90.f;

	//Functions

private:

	UFUNCTION()
	void LoseTarget();

public:

	AOPAG_AICBase();

	void OnPossess(APawn* ControlledPawn);
	
	UFUNCTION()
	void OnSightUpdated(AActor* UpdatedActor, FAIStimulus Stimulus);	

};
