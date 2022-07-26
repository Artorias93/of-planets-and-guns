#include "Characters/Enemies/OPAG_Enemy.h"
#include <GameFramework/CharacterMovementComponent.h>
#include <BehaviorTree/BlackboardComponent.h>
#include <Perception/AISense_Damage.h>
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetMathLibrary.h>
#include <BrainComponent.h>
//My Classes
#include "Characters/Enemies/OPAG_AICBase.h"
#include "Characters/OPAG_AttributeSetBase.h"
#include "Managers/OPAG_EventManagerSubsystem.h"
#include "Managers/OPAG_GameInstance.h"

AOPAG_Enemy::AOPAG_Enemy()
{
	PrimaryActorTick.bCanEverTick = false;
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

void AOPAG_Enemy::ReceiveDamage(float DamageAmount)
{
	Super::ReceiveDamage(DamageAmount);
	
	//Triggers the damage sense on the AI controlled pawn
	//TODO: Does this work???
	UAISense_Damage::ReportDamageEvent(GetWorld(), this,
		UGameplayStatics::GetPlayerPawn(GetWorld(), 0), DamageAmount,
		UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation(),
		GetActorLocation(), NAME_None);
	//TODO: Remove the print to screen
	if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Enemy taking damage! Current health: "+ FString::SanitizeFloat(GetHealth())));
}

//TODO: Remove when the enemy will be able to take damage from the player
void AOPAG_Enemy::DebugTake100Damage()
{
	ReceiveDamage(100.f);
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
	const auto GI = GetGameInstance<UOPAG_GameInstance>();
	if (GI) GI->GetEventManager()->EnemyKilled.Broadcast(this);
	Destroy();
}

void AOPAG_Enemy::GetShootingDirection(FVector& StartLocation, FVector& Direction, float const ShootingAccuracyTolerance /* = 0 */)
{
	if (const AOPAG_AICBase* const AIController = Cast<AOPAG_AICBase>(GetController()))
	{
		if(const AOPAG_CharacterBase* const Player = Cast<AOPAG_CharacterBase>(AIController->GetFocusActor()))
		{
			//Gets Start location, in front of the gun
			StartLocation = GetActorLocation() + (GetActorForwardVector() * 50.f);

			//Gets Vector from player to enemy
			const FVector PlayerToSelfVector = Player->GetActorLocation() - GetActorLocation();

			//Gets not normalized vector with shooting accuracy computations applied
			const float DirectionVectorX =
				PlayerToSelfVector.X + UKismetMathLibrary::RandomFloatInRange(
					-ShootingAccuracyTolerance, ShootingAccuracyTolerance);
			const float DirectionVectorY = PlayerToSelfVector.Y;
			const float DirectionVectorZ =
				PlayerToSelfVector.Z + UKismetMathLibrary::RandomFloatInRange(
					-ShootingAccuracyTolerance, ShootingAccuracyTolerance);
			const FVector DirectionVector = FVector(DirectionVectorX, DirectionVectorY, DirectionVectorZ);
			
			//Gets normalized vector with shooting accuracy computations applied
			Direction = UKismetMathLibrary::Normal(DirectionVector, 0.0001f); //2nd param is tolerance
		}
	}
}