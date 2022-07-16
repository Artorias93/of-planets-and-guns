#include "AI/OPAG_TaskFindRandomSpotInRadius.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
//My Classes
#include "Characters/Enemies/OPAG_AICBase.h"
#include "Characters/Enemies/OPAG_Enemy.h"

UOPAG_TaskFindRandomSpotInRadius::UOPAG_TaskFindRandomSpotInRadius(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Find Random Spot In Radius");
	PatrolRadius = 1000.f;
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
				const FVector PatrolLocation = NavSystem->GetRandomReachablePointInRadius(GetWorld(), ActorLocation, PatrolRadius);
				AIController->GetBlackboardComponent()->SetValueAsVector(BBKeyPatrolLocation.SelectedKeyName, PatrolLocation);
				return EBTNodeResult::Succeeded;
			}
			return EBTNodeResult::Failed;
		}
		return EBTNodeResult::Failed;
	}
	return EBTNodeResult::Failed;
}