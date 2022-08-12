#include "Characters/Enemies/OPAG_Enemy.h"
#include <GameFramework/CharacterMovementComponent.h>
#include <BehaviorTree/BlackboardComponent.h>
#include <Perception/AISense_Damage.h>
#include <Kismet/GameplayStatics.h>
#include <BrainComponent.h>
//My Classes
#include "Characters/Enemies/OPAG_AICBase.h"
#include "Characters/OPAG_AttributeSetBase.h"
#include "Characters/Player/OPAG_Player.h"
#include "Managers/OPAG_EventManagerSubsystem.h"
#include "Managers/OPAG_GameInstance.h"
#include "Characters/Enemies/OPAG_LootDropSystemComponent.h"
#include "Particles/ParticleSystemComponent.h"

AOPAG_Enemy::AOPAG_Enemy()
{
	PrimaryActorTick.bCanEverTick = false;

	LootDropSystemComponent = CreateDefaultSubobject<UOPAG_LootDropSystemComponent>(TEXT("Loot Drop System Component"));

	SpawnParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle System Component"));
	SpawnParticle->SetupAttachment(GetMesh());
}

void AOPAG_Enemy::BeginPlay()
{
	Super::BeginPlay();

	//Gets bUsingPatrolPath from the instance
	if (AOPAG_AICBase* const AIController = Cast<AOPAG_AICBase>(GetController()))
	{
		AIController->GetBlackboardComponent()->SetValueAsBool("bUsingPatrolPath", bUsingPatrolPath);
	}
}

void AOPAG_Enemy::UpdateSpeed(float const NewSpeed) const
{
	GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
}

void AOPAG_Enemy::UpdateAcceleration(float const NewAcceleration) const
{
	GetCharacterMovement()->MaxAcceleration = NewAcceleration;
}

void AOPAG_Enemy::ReceiveDamage(float DamageAmount)
{
	Super::ReceiveDamage(DamageAmount);
	
	//Triggers the damage sense on the AI controlled pawn
	UAISense_Damage::ReportDamageEvent(GetWorld(), this,
		UGameplayStatics::GetPlayerPawn(GetWorld(), 0), DamageAmount,
		UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation(),
		GetActorLocation(), NAME_None);
	//TODO: Remove the print to screen
	if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Enemy taking damage! Current health: "+ FString::SanitizeFloat(GetHealth())));
}

void AOPAG_Enemy::Attack()
{
	//Empty function for children's overrides
}

void AOPAG_Enemy::Death()
{
	//Stops executing AI tasks
	if (const AOPAG_AICBase* const AIController = Cast<AOPAG_AICBase>(GetController()))
	{
		AIController->GetBrainComponent()->StopLogic("Death");
	}

	//Mesh ragdoll functions
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetSimulatePhysics(true);
	GetWorld()->GetTimerManager().SetTimer(RagdollTimerHandle, this, &AOPAG_Enemy::DropAndDespawn, RagdollDurationSeconds);
}

void AOPAG_Enemy::DropAndDespawn()
{
	//Fill with everything from drops to enemy manager stuff

	GetWorld()->GetTimerManager().ClearTimer(RagdollTimerHandle);
	//Broadcasts the kill to the event manager
	const auto GI = GetGameInstance<UOPAG_GameInstance>();
	if (GI) GI->GetEventManager()->EnemyKilled.Broadcast(this);
	Destroy();
}

#pragma region Getters & Setters

UBehaviorTree* AOPAG_Enemy::GetBehaviorTree() const
{
	return BehaviorTree;
}

AOPAG_PatrolPath* AOPAG_Enemy::GetPatrolPath() const
{
	return PatrolPath;
}

#pragma endregion