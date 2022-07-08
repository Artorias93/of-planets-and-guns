#include "AI/OPAG_ServiceComputeDistance.h"
#include "Characters/Enemies/OPAG_AICBase.h"
#include "Characters/Player/OPAG_Player.h"
//Components
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

//BT Service: Computes distance between itself and BB's TargetActor (the player), 
//saving the result in BB's EnemyDistance

UOPAG_ServiceComputeDistance::UOPAG_ServiceComputeDistance(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Compute Distance");
	bNotifyTick = true;
}

void UOPAG_ServiceComputeDistance::TickNode(UBehaviorTreeComponent& OwnerBTComponent, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerBTComponent, NodeMemory, DeltaSeconds);

	if(AOPAG_AICBase* const AIController = Cast<AOPAG_AICBase>(OwnerBTComponent.GetAIOwner()))
	{
		if (AOPAG_Player* const PlayerActor = Cast<AOPAG_Player>(AIController->GetBlackboardComponent()->GetValueAsObject("TargetActor")))
		{
			FVector SelfLocation = AIController->GetPawn()->GetActorLocation();
			FVector PlayerLocation = PlayerActor->GetActorLocation();
			float ActorsDistance = FVector::Distance(SelfLocation, PlayerLocation);

			AIController->GetBlackboardComponent()->SetValueAsFloat("EnemyDistance", ActorsDistance);
		}
	}	
}