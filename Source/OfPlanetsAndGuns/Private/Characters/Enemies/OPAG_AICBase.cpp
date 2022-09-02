#include "Characters/Enemies/OPAG_AICBase.h"
#include "TimerManager.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISenseConfig_Hearing.h"
//My classes
#include "Characters/Enemies/OPAG_Enemy.h"
#include "Characters/Player/OPAG_Player.h"
#include "Weapons/Guns/OPAG_GunBase.h"

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
	PerceptionHearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("AI Hearing Config"));
	
	PerceptionSightConfig->DetectionByAffiliation.bDetectEnemies = true;
	PerceptionSightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	PerceptionSightConfig->DetectionByAffiliation.bDetectEnemies = true;
	PerceptionHearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
	PerceptionHearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
	PerceptionHearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
	PerceptionComp->ConfigureSense(*PerceptionSightConfig);
	PerceptionComp->ConfigureSense(*PerceptionDamageConfig);
	PerceptionComp->ConfigureSense(*PerceptionHearingConfig);
	PerceptionComp->SetDominantSense(PerceptionSightConfig->GetSenseImplementation());
}

void AOPAG_AICBase::OnPossess(APawn* ControlledPawn)
{
	Super::OnPossess(ControlledPawn);

	//We need the AIControlledPawn pointer in the OnPerceptionUpdated method, so we set this class variable
	AIControlledPawn = ControlledPawn;

	const AOPAG_Enemy* const ControlledEnemy = Cast<AOPAG_Enemy>(ControlledPawn);
	if(ControlledEnemy && ControlledEnemy->GetBehaviorTree())
	{
		BlackboardComponent->InitializeBlackboard(*ControlledEnemy->GetBehaviorTree()->BlackboardAsset);
		BehaviorTreeComponent->StartTree(*ControlledEnemy->GetBehaviorTree());

		PerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &AOPAG_AICBase::OnPerceptionUpdated);

		GetBlackboardComponent()->SetValueAsObject("SelfActor", ControlledPawn);
	}
}

void AOPAG_AICBase::OnPerceptionUpdated(AActor* UpdatedActor, FAIStimulus Stimulus)
{	
	//If the stimuli is of type hearing, the bot goes to the source of the sound (without setting its TargetActor)
	if(Stimulus.Type == UAISense::GetSenseID<UAISense_Hearing>() && UpdatedActor != AIControlledPawn)
	{
		GetBlackboardComponent()->SetValueAsVector("TargetLocation", UpdatedActor->GetActorLocation());
	}
	//Sight and damage senses only react to the player character
	else if (AOPAG_Player* const Player = Cast<AOPAG_Player>(UpdatedActor))
	{
		//If the player is seen set it to target actor
		if (Stimulus.WasSuccessfullySensed())
		{
			GetWorld()->GetTimerManager().ClearTimer(LoseSightTimerHandle);
			GetBlackboardComponent()->SetValueAsObject("TargetActor", Player);
			SetFocus(Player, EAIFocusPriority::Gameplay);
		}
		//If the player is lost remove it from target actor after a certain number of seconds
		else
		{
			GetWorld()->GetTimerManager().SetTimer(LoseSightTimerHandle, this,
				&AOPAG_AICBase::LoseTarget, LoseSightTimer);
		}
	}
}

void AOPAG_AICBase::LoseTarget()
{
	GetBlackboardComponent()->SetValueAsObject("TargetActor", nullptr);
	ClearFocus(EAIFocusPriority::Gameplay);
}

void AOPAG_AICBase::SetBlackboardEntryAsBool(FName EntryName, bool Value)
{
	BlackboardComponent->SetValueAsBool(EntryName, Value);
}
