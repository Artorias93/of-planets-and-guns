#include "AI/OPAG_TaskClearBlackboardKey.h"
//My Classes
#include "BehaviorTree/BlackboardComponent.h"

UOPAG_TaskClearBlackboardKey::UOPAG_TaskClearBlackboardKey(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Clear Blackboard Key");
}

EBTNodeResult::Type UOPAG_TaskClearBlackboardKey::ExecuteTask(UBehaviorTreeComponent& OwnerBTComponent, uint8* NodeMemory)
{
	OwnerBTComponent.GetBlackboardComponent()->ClearValue(BBKeyToClear.SelectedKeyName);
	return EBTNodeResult::Succeeded;
}