// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/OPAG_ServiceCastAbility.h"
#include "Characters/Enemies/MageEnemy/OPAG_MageEnemy.h"
#include "Characters/Enemies/OPAG_AICBase.h"
#include "Characters/Player/OPAG_Player.h"

UOPAG_ServiceCastAbility::UOPAG_ServiceCastAbility(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Cast Ability");
	bNotifyTick = true;
	bNotifyBecomeRelevant = true;
	bCreateNodeInstance = true; //This way it create separate instances for the service for different objects, and the timer works as intended
}

void UOPAG_ServiceCastAbility::TickNode(UBehaviorTreeComponent& OwnerBTComponent, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerBTComponent, NodeMemory, DeltaSeconds);

		if (const AOPAG_AICBase* const AIController = Cast<AOPAG_AICBase>(OwnerBTComponent.GetAIOwner()))
		{
			if (const AOPAG_Player* const FocusActor = Cast<AOPAG_Player>(AIController->GetFocusActor()))
			{
				if (AOPAG_MageEnemy* const Enemy = Cast<AOPAG_MageEnemy>(AIController->GetPawn()))
				{
					Enemy->CastAbilityRequest(AbilityType, Enemy, FocusActor, AbilitySpawnPattern);
				}
			}
		}
	
}

void UOPAG_ServiceCastAbility::OnBecomeRelevant(UBehaviorTreeComponent& OwnerBTComponent, uint8* NodeMemory)
{
	
}
