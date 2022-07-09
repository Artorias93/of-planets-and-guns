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
	if (AOPAG_AICBase* const AIController = Cast<AOPAG_AICBase>(OwnerBTComponent.GetAIOwner()))
	{
		if (AOPAG_Enemy* const Enemy = Cast<AOPAG_Enemy>(AIController->GetPawn()))
		{
			Enemy->SetActorRotation(
				UKismetMathLibrary::FindLookAtRotation(Enemy->GetActorLocation(), 
				AIController->GetFocusActor()->GetActorLocation())
			);

			//TODO: This function is empty ATM, both in Enemy and Children
			Enemy->Attack();

			UE_LOG(LogTemp, Warning, TEXT("In attack function"));

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