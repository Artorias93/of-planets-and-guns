#include "AI/OPAG_TaskChangeChargeState.h"
#include "Components/CapsuleComponent.h"
//My Classes
#include "Characters/Enemies/OPAG_AICBase.h"
#include "Characters/Enemies/ShieldEnemy/OPAG_ShieldEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"

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
					FAttachmentTransformRules::SnapToTargetNotIncludingScale, "TopHeadSocket");
				ShieldEnemy->TurnOnBoosters();
				ShieldEnemy->GetCharacterMovement()->GravityScale = 0.f;
				//Controller's rotation is not used anymore
				//ShieldEnemy->bUseControllerRotationYaw = false;
			}
			else
			{
				//Controller's rotation is not used anymore
				//ShieldEnemy->bUseControllerRotationYaw = true;
				ShieldEnemy->GetCharacterMovement()->GravityScale = 1.f;
				ShieldEnemy->TurnOffBoosters();
				ShieldEnemy->GetShieldMesh()->AttachToComponent(ShieldEnemy->GetMesh(),
					FAttachmentTransformRules::SnapToTargetNotIncludingScale, "LeftHandSocket");
				ShieldEnemy->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);				
			}
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
