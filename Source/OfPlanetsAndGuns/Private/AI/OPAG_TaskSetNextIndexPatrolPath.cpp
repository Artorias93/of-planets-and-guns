#include "AI/OPAG_TaskSetNextIndexPatrolPath.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
//My Classes
#include "Characters/Enemies/OPAG_AICBase.h"
#include "Characters/Enemies/OPAG_Enemy.h"
#include "AI/OPAG_PatrolPath.h"

UOPAG_TaskSetNextIndexPatrolPath::UOPAG_TaskSetNextIndexPatrolPath(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Set Next Index In Patrol Path");
}

EBTNodeResult::Type UOPAG_TaskSetNextIndexPatrolPath::ExecuteTask(UBehaviorTreeComponent& OwnerBTComponent, uint8* NodeMemory)
{
	if (AOPAG_AICBase* const AIController = Cast<AOPAG_AICBase>(OwnerBTComponent.GetAIOwner()))
	{
		if (AOPAG_Enemy* const Enemy = Cast<AOPAG_Enemy>(AIController->GetPawn()))
		{
			int CurrentPatrolPathIndex = AIController->GetBlackboardComponent()->GetValueAsInt(BBKeyPatrolPathIndex.SelectedKeyName);
			TArray<FVector> const PatrolPathPointsArray = Enemy->PatrolPath->PatrolPathPoints;

			//If we are at the last index go back to 0, else increment
			if(CurrentPatrolPathIndex >= PatrolPathPointsArray.Num() - 1)
			{
				AIController->GetBlackboardComponent()->SetValueAsInt(BBKeyPatrolPathIndex.SelectedKeyName, 0);
				CurrentPatrolPathIndex = 0;
			}
			else
			{
				AIController->GetBlackboardComponent()->SetValueAsInt(BBKeyPatrolPathIndex.SelectedKeyName, ++CurrentPatrolPathIndex);
			}

			//Convert patrol path point location from local coordinates 
			//(relative to the patrol path object) to world coordinates
			FVector PointWorldLocation = UKismetMathLibrary::TransformLocation(Enemy->PatrolPath->GetTransform(), 
				PatrolPathPointsArray[CurrentPatrolPathIndex]);

			AIController->GetBlackboardComponent()->SetValueAsVector(BBKeyPatrolLocation.SelectedKeyName, PointWorldLocation);

			FinishLatentTask(OwnerBTComponent, EBTNodeResult::Succeeded);
			return EBTNodeResult::Succeeded;
		}
		else
		{
			FinishLatentTask(OwnerBTComponent, EBTNodeResult::Failed);
			return EBTNodeResult::Failed;
		}
	}
	else
	{
		FinishLatentTask(OwnerBTComponent, EBTNodeResult::Failed);
		return EBTNodeResult::Failed;
	}
}