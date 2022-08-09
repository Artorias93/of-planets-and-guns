#include "AI/OPAG_TaskCharge.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemies/OPAG_AICBase.h"
#include "Characters/Enemies/ShieldEnemy/OPAG_ShieldEnemy.h"
#include "Characters/Player/OPAG_Player.h"
#include "Kismet/KismetSystemLibrary.h"

UOPAG_TaskCharge::UOPAG_TaskCharge(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Set Charge Location");
}

EBTNodeResult::Type UOPAG_TaskCharge::ExecuteTask(UBehaviorTreeComponent& OwnerBTComponent, uint8* NodeMemory)
{
	if (AOPAG_AICBase* const AIController = Cast<AOPAG_AICBase>(OwnerBTComponent.GetAIOwner()))
	{
		if (AOPAG_ShieldEnemy* const ShieldEnemy = Cast<AOPAG_ShieldEnemy>(AIController->GetPawn()))
		{
			if(const AOPAG_Player* const Player = Cast<AOPAG_Player>(AIController->GetFocusActor()))
			{	
				const FVector EnemyPos = ShieldEnemy->GetActorLocation();	
				const FVector PlayerPos = Player->GetActorLocation();

				FVector OutDir;
				float OutLength;			
				(PlayerPos - EnemyPos).ToDirectionAndLength(OutDir, OutLength);

				const FVector ChargePoint = EnemyPos + OutDir * (OutLength + ChargeDistanceBehindPlayer);
				
				FHitResult Hit;
				const TArray<AActor*> ActorsToIgnore;

				if(!UKismetSystemLibrary::BoxTraceSingle(GetWorld(), EnemyPos, ChargePoint,
					BoxTraceHalfSize, FRotator::ZeroRotator, TraceTypeQuery1,
					false, ActorsToIgnore, EDrawDebugTrace::ForDuration, Hit, true))
				{
					if(GEngine)
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("OBSTACLE NOT FOUND"));

					//TODO: Remove if everything is inside the charge BTTask
					//AIController->GetBlackboardComponent()->SetValueAsVector(BBKeyTargetLocation.SelectedKeyName, ChargePoint);

					ShieldEnemy->SetIsCharging(true);
					AIController->MoveToLocation(ChargePoint, 100.f, false, false,
						false, false, nullptr, false);
					ShieldEnemy->SetIsCharging(false);

					return EBTNodeResult::Succeeded;
				}

				if(GEngine)
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("OBSTACLE FOUND"));
			}			
		}
	}
	return EBTNodeResult::Failed;
}