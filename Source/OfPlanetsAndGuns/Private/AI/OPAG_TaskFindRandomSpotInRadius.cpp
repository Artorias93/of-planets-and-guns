#include "AI/OPAG_TaskFindRandomSpotInRadius.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
//My Classes
#include "Characters/Enemies/OPAG_AICBase.h"
#include "Characters/Enemies/OPAG_Enemy.h"

UOPAG_TaskFindRandomSpotInRadius::UOPAG_TaskFindRandomSpotInRadius(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Find Random Spot In Radius");
}

EBTNodeResult::Type UOPAG_TaskFindRandomSpotInRadius::ExecuteTask(UBehaviorTreeComponent& OwnerBTComponent, uint8* NodeMemory)
{
	if(AOPAG_AICBase* const AIController = Cast<AOPAG_AICBase>(OwnerBTComponent.GetAIOwner()))
	{
		if (const AOPAG_Enemy* const Enemy = Cast<AOPAG_Enemy>(AIController->GetPawn()))
		{
			const FVector ActorLocation = Enemy->GetActorLocation();
			if (const UNavigationSystemV1* const NavSystem = UNavigationSystemV1::GetCurrent(GetWorld()))
			{
				//Gets the random point and writes it on the blackboard
				FNavLocation PatrolLocation;
				if(NavSystem->GetRandomPointInNavigableRadius(ActorLocation, PatrolRadius, PatrolLocation))
				{
					AIController->GetBlackboardComponent()->SetValueAsVector(BBKeyPatrolLocation.SelectedKeyName,
					PatrolLocation.Location);
					return EBTNodeResult::Succeeded;
				}
			}
		}
	}
	return EBTNodeResult::Failed;
}