#include "AI/OPAG_TaskMoveDirectlyToward.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemies/OPAG_AICBase.h"
#include "Characters/Enemies/OPAG_Enemy.h"
#include "Characters/Player/OPAG_Player.h"
#include "Kismet/KismetMathLibrary.h"

UOPAG_TaskMoveDirectlyToward::UOPAG_TaskMoveDirectlyToward(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("OPAGMoveDirectlyToward");
	bNotifyTick = true;
	//bNotifyTaskFinished = true;
}

EBTNodeResult::Type UOPAG_TaskMoveDirectlyToward::ExecuteTask(UBehaviorTreeComponent& OwnerBTComponent, uint8* NodeMemory)
{
	AlphaLerp = 0;

	if (const AOPAG_AICBase* const AIController = Cast<AOPAG_AICBase>(OwnerBTComponent.GetAIOwner()))
	{
		if (const AOPAG_Enemy* const Agent = Cast<AOPAG_Enemy>(AIController->GetPawn()))
		{
			AgentLocation = Agent->GetActorLocation();
			
			ChargeLoc = AIController->GetBlackboardComponent()->GetValueAsVector(
				BBKeyChargeLocation.SelectedKeyName);

			Distance = (ChargeLoc - AgentLocation).Size();
		}
	}	
	return EBTNodeResult::InProgress;	
}

void UOPAG_TaskMoveDirectlyToward::TickTask(UBehaviorTreeComponent& OwnerBTComponent, uint8* NodeMemory, float DeltaSeconds)
{
	if (const AOPAG_AICBase* const AIController = Cast<AOPAG_AICBase>(OwnerBTComponent.GetAIOwner()))
	{
		if (AOPAG_Enemy* const Agent = Cast<AOPAG_Enemy>(AIController->GetPawn()))
		{
			AlphaLerp += MoveSpeed * DeltaSeconds / Distance;
			const FVector NewPosition = UKismetMathLibrary::VLerp(AgentLocation, ChargeLoc, AlphaLerp);
			Agent->SetActorLocation(NewPosition);

			if(AlphaLerp >= 1)
			{
				FinishLatentTask(OwnerBTComponent, EBTNodeResult::Succeeded);
			}
		}
	}	
}