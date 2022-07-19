#include "AI/OPAG_TaskAttack.h"
#include <Kismet/KismetMathLibrary.h>
//My Classes
#include "Characters/Enemies/OPAG_AICBase.h"
#include "Characters/Enemies/OPAG_Enemy.h"

UOPAG_TaskAttack::UOPAG_TaskAttack(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Attack");
}

EBTNodeResult::Type UOPAG_TaskAttack::ExecuteTask(UBehaviorTreeComponent& OwnerBTComponent, uint8* NodeMemory)
{
	if (const AOPAG_AICBase* const AIController = Cast<AOPAG_AICBase>(OwnerBTComponent.GetAIOwner()))
	{
		if (AOPAG_Enemy* const Enemy = Cast<AOPAG_Enemy>(AIController->GetPawn()))
		{
			//Face the player
			Enemy->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(Enemy->GetActorLocation(), 
				AIController->GetFocusActor()->GetActorLocation())
			);
			Enemy->Attack();
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}