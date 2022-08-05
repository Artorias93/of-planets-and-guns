#include "AI/OPAG_TaskChangeAcceleration.h"
//My Classes
#include "Characters/Enemies/OPAG_AICBase.h"
#include "Characters/Enemies/OPAG_Enemy.h"

UOPAG_TaskChangeAcceleration::UOPAG_TaskChangeAcceleration(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Change Acceleration");
}

EBTNodeResult::Type UOPAG_TaskChangeAcceleration::ExecuteTask(UBehaviorTreeComponent& OwnerBTComponent, uint8* NodeMemory)
{
	if(const AOPAG_AICBase* const AIController = Cast<AOPAG_AICBase>(OwnerBTComponent.GetAIOwner()))
	{
		if (AOPAG_Enemy* const Enemy = Cast<AOPAG_Enemy>(AIController->GetPawn()))
		{
			Enemy->UpdateAcceleration(NewAcceleration);
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}