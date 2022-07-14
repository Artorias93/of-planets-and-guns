#include "AI/OPAG_TaskMoveTo.h"
#include "BehaviorTree/BlackboardComponent.h"
//My Classes
#include "Characters/Enemies/OPAG_AICBase.h"


UOPAG_TaskMoveTo::UOPAG_TaskMoveTo(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Move To");
}

EBTNodeResult::Type UOPAG_TaskMoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerBTComponent, uint8* NodeMemory)
{
	if (AOPAG_AICBase* const AIController = Cast<AOPAG_AICBase>(OwnerBTComponent.GetAIOwner()))
	{
		/*UObject* Destination = AIController->GetBlackboardComponent()->GetValueAsObject(BBKeyDestination.SelectedKeyName);

		if (FVector* const DestinationLocation = Cast<FVector>(Destination))
		{
			AIController->MoveToLocation(*DestinationLocation, AcceptedRadius);
			FinishLatentTask(OwnerBTComponent, EBTNodeResult::Succeeded);
			return EBTNodeResult::Succeeded;
		}
		else if (AActor* DestinationActor = Cast<AActor>(Destination))
		{
			AIController->MoveToActor(DestinationActor, AcceptedRadius);
			FinishLatentTask(OwnerBTComponent, EBTNodeResult::Succeeded);
			return EBTNodeResult::Succeeded;
		}
		FinishLatentTask(OwnerBTComponent, EBTNodeResult::Failed);
		return EBTNodeResult::Failed;*/

		FinishLatentTask(OwnerBTComponent, EBTNodeResult::Failed);
		return EBTNodeResult::Failed;
	}
	else
	{
		FinishLatentTask(OwnerBTComponent, EBTNodeResult::Failed);
		return EBTNodeResult::Failed;
	}
}