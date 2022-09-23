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
	if (AOPAG_AICBase* const AIController = Cast<AOPAG_AICBase>(OwnerBTComponent.GetAIOwner()))
	{
		if (AOPAG_ShieldEnemy* const ShieldEnemy = Cast<AOPAG_ShieldEnemy>(AIController->GetPawn()))
		{
			ShieldEnemy->SetIsCharging(bIsCharging);

			if(bIsCharging)
			{
				CollisionValues = ShieldEnemy->GetCapsuleComponent()->GetCollisionResponseToChannels();
				ShieldEnemy->GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Overlap);
				ShieldEnemy->GetShieldMesh()->AttachToComponent(ShieldEnemy->GetMesh(),
					FAttachmentTransformRules::SnapToTargetNotIncludingScale, "TopHeadSocket");
				ShieldEnemy->TurnOnBoosters();
				ShieldEnemy->GetCharacterMovement()->GravityScale = 0.f;
				ShieldEnemy->bUseControllerRotationYaw = true;
			}
			else
			{
				ShieldEnemy->bUseControllerRotationYaw = false;
				ShieldEnemy->GetCharacterMovement()->GravityScale = 1.f;
				ShieldEnemy->TurnOffBoosters();
				ShieldEnemy->GetShieldMesh()->AttachToComponent(ShieldEnemy->GetMesh(),
					FAttachmentTransformRules::SnapToTargetNotIncludingScale, "LeftHandSocket");
				ShieldEnemy->GetCapsuleComponent()->SetCollisionResponseToChannels(CollisionValues);
				//TODO: The previous line should do it before this, but doesn't and I don't know why.
				ShieldEnemy->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
			}
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}