#include "AI/OPAG_TaskMoveTo.h"
#include "BehaviorTree/BlackboardComponent.h"
#include <BehaviorTree/Blackboard/BlackboardKeyType_Object.h>
#include <BehaviorTree/Blackboard/BlackboardKeyType_Vector.h>
//My Classes
#include "Characters/Enemies/OPAG_AICBase.h"

//---------------------------------------------------------------------
//THIS TASK IS CURRENTLY UNUSED
//---------------------------------------------------------------------

UOPAG_TaskMoveTo::UOPAG_TaskMoveTo(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Move To");
}

EBTNodeResult::Type UOPAG_TaskMoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerBTComponent, uint8* NodeMemory)
{
	if (AOPAG_AICBase* const AIController = Cast<AOPAG_AICBase>(OwnerBTComponent.GetAIOwner()))
	{
		if(const UBlackboardComponent* const BlackBoard = Cast<UBlackboardComponent>(OwnerBTComponent.GetBlackboardComponent()))
		{			

			if (AActor* const DestinationActor = Cast<AActor>(BlackBoard->GetValue<UBlackboardKeyType_Object>(BBKeyDestination.SelectedKeyName)))
			{
				AIController->MoveToActor(DestinationActor, AcceptedRadius);
				FinishLatentTask(OwnerBTComponent, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
			if (BBKeyDestination.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
			{
				const FVector DestinationLocation = BlackBoard->GetValueAsVector(BBKeyDestination.SelectedKeyName);
				AIController->MoveToLocation(DestinationLocation, AcceptedRadius);
				return EBTNodeResult::Succeeded;
			}
			return EBTNodeResult::Failed;
		}
		return EBTNodeResult::Failed;		
	}
	return EBTNodeResult::Failed;
}