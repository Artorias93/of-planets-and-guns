#include "AI/OPAG_TaskChangeSpeed.h"
//My Classes
#include "Characters/Enemies/OPAG_AICBase.h"
#include "Characters/Enemies/OPAG_Enemy.h"

UOPAG_TaskChangeSpeed::UOPAG_TaskChangeSpeed(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Change Speed");
}

EBTNodeResult::Type UOPAG_TaskChangeSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerBTComponent, uint8* NodeMemory)
{
	if(AOPAG_AICBase* const AIController = Cast<AOPAG_AICBase>(OwnerBTComponent.GetAIOwner()))
	{
		if (AOPAG_Enemy* const Enemy = Cast<AOPAG_Enemy>(AIController->GetPawn()))
		{
			Enemy->UpdateSpeed(NewSpeed);
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