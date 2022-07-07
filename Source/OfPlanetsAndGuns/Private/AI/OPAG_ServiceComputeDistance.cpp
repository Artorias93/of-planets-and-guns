#include "AI/OPAG_ServiceComputeDistance.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UOPAG_ServiceComputeDistance::UOPAG_ServiceComputeDistance(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Compute Distance");

	bNotifyTick = true;
}

void UOPAG_ServiceComputeDistance::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	//Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AIController = Cast<AAIController>(OwnerComp.GetAIOwner());

	//Add check on enemy class?
	//AActor? APawn? ACharacter?
	if(AActor* OtherActor = Cast<AActor>(AIController->GetBlackboardComponent()->GetValueAsObject("TargetActor")))
	{
		FVector SelfLocation = AIController->GetPawn()->GetActorLocation();
		FVector OtherLocation = OtherActor->GetActorLocation();

		//Check in source
		float ActorsDistance = FVector::Distance(SelfLocation, OtherLocation);

		AIController->GetBlackboardComponent()->SetValueAsFloat("EnemyDistance", ActorsDistance);
	}
}