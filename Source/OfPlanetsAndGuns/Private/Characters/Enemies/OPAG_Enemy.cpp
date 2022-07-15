#include "Characters/Enemies/OPAG_Enemy.h"
#include <GameFramework/CharacterMovementComponent.h>
#include <BehaviorTree/BlackboardComponent.h>
#include <Perception/AISense_Damage.h>
#include <Kismet/GameplayStatics.h>
#include <BrainComponent.h>
//My Classes
#include "Characters/Enemies/OPAG_AICBase.h"
#include "Characters/OPAG_AttributeSetBase.h"

AOPAG_Enemy::AOPAG_Enemy()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AOPAG_Enemy::BeginPlay()
{
	Super::BeginPlay();

	if (AOPAG_AICBase* const AIController = Cast<AOPAG_AICBase>(GetController()))
	{
		AIController->GetBlackboardComponent()->SetValueAsBool("bUsingPatrolPath", bUsingPatrolPath);
	}
}

void AOPAG_Enemy::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AOPAG_Enemy::UpdateSpeed(float NewSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
}

void AOPAG_Enemy::TakeDamage(float DamageAmount)
{
	Super::TakeDamage(DamageAmount);
	//Triggers the damage sense on the AI controlled pawn
	UAISense_Damage::ReportDamageEvent(GetWorld(), this, UGameplayStatics::GetPlayerPawn(GetWorld(), 0), DamageAmount, UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation(), GetActorLocation(), NAME_None);
	if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Player taking damage!"));
}

void AOPAG_Enemy::Attack()
{
	//Empty function for children's overrides
}

void AOPAG_Enemy::Death()
{
	//Stops executing AI tasks
	if (AOPAG_AICBase* const AIController = Cast<AOPAG_AICBase>(GetController()))
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
	Destroy();
}