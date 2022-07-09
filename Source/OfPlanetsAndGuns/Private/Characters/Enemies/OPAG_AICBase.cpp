#include "Characters/Enemies/OPAG_AICBase.h"
#include "TimerManager.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Damage.h"
//My classes
#include "Characters/Enemies/OPAG_Enemy.h"
#include "Characters/Player/OPAG_Player.h"

AOPAG_AICBase::AOPAG_AICBase()
{
	bStartAILogicOnPossess = true;

	//Components Setup
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("AIC Blackboard Component"));
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("AIC BehaviorTree Component"));

	//AIPerception Setup
	PerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception Component"));
	PerceptionSightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("AI Sight Config"));
	PerceptionDamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("AI Damage Config"));
	//TODO: Move this setup in the specialized sub-classes
	PerceptionSightConfig->SightRadius = 3000.f;
	PerceptionSightConfig->LoseSightRadius = 3500.f;
	PerceptionSightConfig->PeripheralVisionAngleDegrees = 90.0f;
	PerceptionSightConfig->DetectionByAffiliation.bDetectEnemies = true;
	PerceptionSightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	PerceptionSightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	PerceptionComp->ConfigureSense(*PerceptionSightConfig);
	PerceptionComp->ConfigureSense(*PerceptionDamageConfig);
	PerceptionComp->SetDominantSense(PerceptionSightConfig->GetSenseImplementation());
}

void AOPAG_AICBase::OnPossess(APawn* ControlledPawn)
{
	Super::OnPossess(ControlledPawn);

	AOPAG_Enemy* const ControlledEnemy = Cast<AOPAG_Enemy>(ControlledPawn);
	if(ControlledEnemy && ControlledEnemy->BehaviorTree)
	{
		BlackboardComponent->InitializeBlackboard(*ControlledEnemy->BehaviorTree->BlackboardAsset);
		BehaviorTreeComponent->StartTree(*ControlledEnemy->BehaviorTree);

		PerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &AOPAG_AICBase::OnPerceptionUpdated);
	}
}

void AOPAG_AICBase::OnPerceptionUpdated(AActor* UpdatedActor, FAIStimulus Stimulus)
{
	//The sight sense only reacts to the player character
	if (AOPAG_Player* const Player = Cast<AOPAG_Player>(UpdatedActor))
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			GetWorld()->GetTimerManager().ClearTimer(LoseSightTimerHandle);
			GetBlackboardComponent()->SetValueAsObject("EnemyActor", Player);
			SetFocus(Player, EAIFocusPriority::Gameplay);
		}
		else
		{
			GetWorld()->GetTimerManager().SetTimer(LoseSightTimerHandle, this, &AOPAG_AICBase::LoseTarget, LoseSightTimer);
		}
	}
}

void AOPAG_AICBase::LoseTarget()
{
	GetBlackboardComponent()->SetValueAsObject("EnemyActor", nullptr);
	ClearFocus(EAIFocusPriority::Gameplay);
}