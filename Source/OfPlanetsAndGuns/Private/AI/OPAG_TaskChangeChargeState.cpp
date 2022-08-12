#include "AI/OPAG_TaskChangeChargeState.h"

#include "Characters/Enemies/OPAG_AICBase.h"
#include "Characters/Enemies/ShieldEnemy/OPAG_ShieldEnemy.h"
#include "Components/CapsuleComponent.h"

UOPAG_TaskChangeChargeState::UOPAG_TaskChangeChargeState(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Change Charge State");
}

EBTNodeResult::Type UOPAG_TaskChangeChargeState::ExecuteTask(UBehaviorTreeComponent& OwnerBTComponent, uint8* NodeMemory)
{
	if (const AOPAG_AICBase* const AIController = Cast<AOPAG_AICBase>(OwnerBTComponent.GetAIOwner()))
	{
		if (AOPAG_ShieldEnemy* const ShieldEnemy = Cast<AOPAG_ShieldEnemy>(AIController->GetPawn()))
		{
			ShieldEnemy->SetIsCharging(bIsCharging);

			if(bIsCharging)
			{
				ShieldEnemy->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
				ShieldEnemy->GetShieldMesh()->AttachToComponent(ShieldEnemy->GetMesh(),
					FAttachmentTransformRules::SnapToTargetNotIncludingScale, "ChargingShieldSocket");
				ShieldEnemy->TurnOnBoosters();
			}
			else
			{
				ShieldEnemy->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
				ShieldEnemy->GetShieldMesh()->AttachToComponent(ShieldEnemy->GetMesh(),
					FAttachmentTransformRules::SnapToTargetNotIncludingScale, "LeftHandSocket");
				ShieldEnemy->TurnOffBoosters();
			}
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
