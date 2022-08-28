// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/OPAG_TaskCastAbility.h"
#include "Characters/Enemies/OPAG_AICBase.h"
#include "Characters/Player/OPAG_Player.h"
#include "Characters/Enemies/MageEnemy/OPAG_MageEnemy.h"

UOPAG_TaskCastAbility::UOPAG_TaskCastAbility(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Cast Ability");

	
}

EBTNodeResult::Type UOPAG_TaskCastAbility::ExecuteTask(UBehaviorTreeComponent& OwnerBTComponent, uint8* NodeMemory)
{
	if (const AOPAG_AICBase* const AIController = Cast<AOPAG_AICBase>(OwnerBTComponent.GetAIOwner()))
	{
		if (AOPAG_Player* FocusActor = Cast<AOPAG_Player>(AIController->GetFocusActor()))
		{
			if (AOPAG_MageEnemy* const Enemy = Cast<AOPAG_MageEnemy>(AIController->GetPawn()))
			{
				float randomIndex = FMath::RandRange(0, AvailableAbilitySpawnPattern.Num()-1);
				Enemy->CastAbilityRequest(AbilityType, Enemy, FocusActor, AvailableAbilitySpawnPattern[randomIndex]);

				return EBTNodeResult::Succeeded;
			}
		}
	}

	return EBTNodeResult::Failed;
}
