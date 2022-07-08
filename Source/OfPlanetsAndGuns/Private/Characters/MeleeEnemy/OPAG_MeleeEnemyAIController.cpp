#include "Characters/MeleeEnemy/OPAG_MeleeEnemyAIController.h"

AOPAG_MeleeEnemyAIController::AOPAG_MeleeEnemyAIController()
{
	//bStartAILogicOnPossess = true;
	//bStopAILogicOnUnposses = true;

	//Components Setup
	/*BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("AIC Blackboard Component"));
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("AIC BehaviorTree Component"));*/
}

void AOPAG_MeleeEnemyAIController::OnPossess(APawn* ControlledPawn)
{
	Super::OnPossess(ControlledPawn);

	//Replace with APrototypeEnemy when I make it
	APawn* ControlledPrototypeEnemy = Cast<APawn>(ControlledPawn);
	/*if(ControlledPrototypeEnemy && ControlledPrototypeEnemy->BehaviorTree)
	{
		BlackboardComponent->InitializeBlackboard(*ControlledPrototypeEnemy->)
	}*/
}



//void AGEPlatformerAIController::OnPossess(APawn* ControlledPawn)
//{
//	Super::OnPossess(ControlledPawn);
//
//	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(ControlledPawn);
//	if (Enemy != nullptr && Enemy->BehaviorTree != nullptr)
//	{
//		BlackboardComponent->InitializeBlackboard(*Enemy->BehaviorTree->BlackboardAsset);
//		BehaviorTreeComponent->StartTree(*Enemy->BehaviorTree);
//
//		//Perception is only allowed when the enemy is a monster enemy (the crab just patrols on a set path)
//		if (AMonsterEnemy* MonsterEnemy = Cast<AMonsterEnemy>(Enemy))
//		{
//			PerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &AGEPlatformerAIController::OnSightUpdated);
//		}
//	}
//}