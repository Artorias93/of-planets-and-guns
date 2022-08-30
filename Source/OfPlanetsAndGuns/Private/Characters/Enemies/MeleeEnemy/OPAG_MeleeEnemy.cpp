#include "Characters/Enemies/MeleeEnemy/OPAG_MeleeEnemy.h"
#include <Components/BoxComponent.h>
//My Classes
#include "Characters/Enemies/OPAG_AICBase.h"
#include "Characters/Player/OPAG_Player.h"

AOPAG_MeleeEnemy::AOPAG_MeleeEnemy()
{
	PrimaryActorTick.bCanEverTick = false;

	//Components creation
	DamageCollisionBox = CreateDefaultSubobject<UBoxComponent>("Damage Collision Box Component");
	if(DamageCollisionBox)
	{
		DamageCollisionBox->SetupAttachment(RootComponent);
		DamageCollisionBox->SetRelativeLocation(DamageCollisionBoxRelativeLocation);
		DamageCollisionBox->SetBoxExtent(DamageCollisionBoxExtent);
		DamageCollisionBox->BodyInstance.SetCollisionProfileName(TEXT("Volume"));
	}
	SwordMesh = CreateDefaultSubobject<UStaticMeshComponent>("Sword Mesh Component");
	if (SwordMesh)
	{
		SwordMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "RightHandSocket");
	}
}

void AOPAG_MeleeEnemy::BeginPlay()
{
	Super::BeginPlay();	

	//Sets delegate functions that disable pawn rotation at the start of the attack animation and disables it at the end.
	//Collision checks and damage computations are only executed when the notify is encountered in the animation.
	AnimInstance = (GetMesh()) ? GetMesh()->GetAnimInstance() : nullptr;
	if (MeleeAttackMontage && AnimInstance)
	{
		AnimInstance->OnMontageStarted.AddDynamic(this, &AOPAG_MeleeEnemy::OnAttackBegin);
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &AOPAG_MeleeEnemy::OnAttackNotify);
		AnimInstance->OnMontageEnded.AddDynamic(this, &AOPAG_MeleeEnemy::OnAttackEnd);
	}
}

void AOPAG_MeleeEnemy::Attack()
{
	AnimInstance->Montage_Play(MeleeAttackMontage, MeleeAttackMontagePlayRate /* = 1.9f */);
}

//Stops the enemy's ability to turn in the direction of the focus actor during the attack animation 
void AOPAG_MeleeEnemy::OnAttackBegin(UAnimMontage* Montage)
{
	bUseControllerRotationYaw = false;
}

//Resumes the enemy's ability to turn in the direction of the focus actor after the attack animation is done
void AOPAG_MeleeEnemy::OnAttackEnd(UAnimMontage* Montage, bool bInterrupted)
{
	bUseControllerRotationYaw = true;
}

//The attack collision & damage is computed only when the animation montage's notify is reached
void AOPAG_MeleeEnemy::OnAttackNotify(FName FunctionName, const FBranchingPointNotifyPayload &NotifyPayload)
{
	if (const AOPAG_AICBase* const AIController = Cast<AOPAG_AICBase>(GetController()))
	{
		if (DamageCollisionBox->IsOverlappingActor(AIController->GetFocusActor()))
		{
			//TODO: Remove the print to screen
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("HIT!"));
			
			if(AOPAG_Player* const Player = Cast<AOPAG_Player>(AIController->GetFocusActor()))
			{
				Player->ReceiveDamage(MeleeWeaponDamage, this);
			}
		}
		else
		{
			//TODO: Remove the print to screen
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("MISS..."));
		}
	}
}

void AOPAG_MeleeEnemy::Death()
{
	//Weapon Mesh ragdoll functions
	if(SwordMesh)
	{
		SwordMesh->K2_DetachFromComponent(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld);
		SwordMesh->SetSimulatePhysics(true);
		SwordMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	}

	//Ragdoll stuff for the enemy mesh itself are done in the parent.
	Super::Death();
}