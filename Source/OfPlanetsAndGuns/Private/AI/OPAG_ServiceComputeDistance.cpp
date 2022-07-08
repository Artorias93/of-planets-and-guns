#include "AI/OPAG_ServiceComputeDistance.h"
#include "AIController.h"
//Components
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

//BT Service: Computes distance between itself and BB's TargetActor, saving the result in BB's EnemyDistance

UOPAG_ServiceComputeDistance::UOPAG_ServiceComputeDistance(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Compute Distance");
	bNotifyTick = true;
}

void UOPAG_ServiceComputeDistance::TickNode(UBehaviorTreeComponent& OwnerBTComponent, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerBTComponent, NodeMemory, DeltaSeconds);

	AAIController* AIController = Cast<AAIController>(OwnerBTComponent.GetAIOwner());

	if(AActor* OtherActor = Cast<AActor>(AIController->GetBlackboardComponent()->GetValueAsObject("TargetActor")))
	{
		FVector SelfLocation = AIController->GetPawn()->GetActorLocation();
		FVector OtherLocation = OtherActor->GetActorLocation();
		float ActorsDistance = FVector::Distance(SelfLocation, OtherLocation);

		AIController->GetBlackboardComponent()->SetValueAsFloat("EnemyDistance", ActorsDistance);
	}
}