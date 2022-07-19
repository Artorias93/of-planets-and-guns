#include "AI/OPAG_ServiceComputeDistance.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
//My Classes
#include "Characters/Enemies/OPAG_AICBase.h"
#include "Characters/OPAG_CharacterBase.h" //Remove when player is ready
#include "Characters/Player/OPAG_Player.h"

//---------------------------------------------------------------------
//THIS TASK IS CURRENTLY UNUSED
//---------------------------------------------------------------------

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
		//Replace with the proper class when it's ready
		if (const AOPAG_CharacterBase* const PlayerActor = Cast<AOPAG_CharacterBase>(
			AIController->GetBlackboardComponent()->GetValueAsObject(BBKeyTargetActor.SelectedKeyName)))
		{
			const FVector SelfLocation = AIController->GetPawn()->GetActorLocation();
			const FVector PlayerLocation = PlayerActor->GetActorLocation();
			const float ActorsDistance = FVector::Distance(SelfLocation, PlayerLocation);

			AIController->GetBlackboardComponent()->SetValueAsFloat(BBKeyEnemyDistance.SelectedKeyName, ActorsDistance);
		}
	}	
}