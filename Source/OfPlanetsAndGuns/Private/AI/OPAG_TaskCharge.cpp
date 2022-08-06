#include "AI/OPAG_TaskCharge.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemies/OPAG_AICBase.h"
#include "Characters/Enemies/OPAG_Enemy.h"
#include "Characters/Player/OPAG_Player.h"

UOPAG_TaskCharge::UOPAG_TaskCharge(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Set Charge Location");
}

EBTNodeResult::Type UOPAG_TaskCharge::ExecuteTask(UBehaviorTreeComponent& OwnerBTComponent, uint8* NodeMemory)
{
	if (AOPAG_AICBase* const AIController = Cast<AOPAG_AICBase>(OwnerBTComponent.GetAIOwner()))
	{
		if (const AOPAG_Enemy* const Enemy = Cast<AOPAG_Enemy>(AIController->GetPawn()))
		{
			if(const AOPAG_Player* const Player = Cast<AOPAG_Player>(AIController->GetFocusActor()))
			{				
				const FVector EnemyPos = Enemy->GetActorLocation();			
				const FVector PlayerPos = Player->GetActorLocation();

				FVector OutDir;
				float OutLength;			
				(PlayerPos - EnemyPos).ToDirectionAndLength(OutDir, OutLength);

				const FVector ChargePoint = EnemyPos + OutDir * (OutLength + ChargeDistance);
				
				AIController->GetBlackboardComponent()->SetValueAsVector(BBKeyTargetLocation.SelectedKeyName,
					ChargePoint);

				return EBTNodeResult::Succeeded;
			}			
		}
	}
	return EBTNodeResult::Failed;
}