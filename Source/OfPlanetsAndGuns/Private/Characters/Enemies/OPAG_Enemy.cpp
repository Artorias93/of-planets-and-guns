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
	PrimaryActorTick.bCanEverTick = true;
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

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::SanitizeFloat(GetHealth()));
}

void AOPAG_Enemy::UpdateSpeed(float NewSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
}

void AOPAG_Enemy::TakeDamage(float DamageAmount)
{
	//Super::TakeDamage(DamageAmount);

	UAISense_Damage::ReportDamageEvent(GetWorld(), this, UGameplayStatics::GetPlayerPawn(GetWorld(), 0), DamageAmount, UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation(), GetActorLocation(), NAME_None);

}

void AOPAG_Enemy::Attack()
{
	//Empty function for children's overrides
	if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("I am attacking!"));
}

void AOPAG_Enemy::Death()
{
	if (AOPAG_AICBase* const AIController = Cast<AOPAG_AICBase>(GetController()))
	{
		AIController->GetBrainComponent()->StopLogic("");
	}

	//Ragdoll functions
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