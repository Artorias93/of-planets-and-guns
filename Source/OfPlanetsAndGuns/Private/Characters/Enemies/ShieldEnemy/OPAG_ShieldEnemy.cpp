#include "Characters/Enemies/ShieldEnemy/OPAG_ShieldEnemy.h"
#include "Components/BoxComponent.h"
//My Classes
#include "Characters/Enemies/OPAG_AICBase.h"
#include "Characters/Player/OPAG_Player.h"

AOPAG_ShieldEnemy::AOPAG_ShieldEnemy()
{
	PrimaryActorTick.bCanEverTick = false;

	//Components creation
	DamageCollisionBox = CreateDefaultSubobject<UBoxComponent>("Attack Damage Collision Box Component");
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
		SwordMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "RightHandSocketShield");
	}

	ShieldMesh = CreateDefaultSubobject<UStaticMeshComponent>("Shield Mesh Component");
	if (ShieldMesh)
	{
		ShieldMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "LeftHandSocket");
	}
}

void AOPAG_ShieldEnemy::BeginPlay()
{
	Super::BeginPlay();

	//Sets delegate functions that disable pawn rotation at the start of the attack animation and disables it at the end.
	//Collision checks and damage computations are only executed when the notify is encountered in the animation.
	AnimInstance = (GetMesh()) ? GetMesh()->GetAnimInstance() : nullptr;
	if (ShieldAttackMontage && AnimInstance)
	{
		AnimInstance->OnMontageStarted.AddDynamic(this, &AOPAG_ShieldEnemy::OnAttackBegin);
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &AOPAG_ShieldEnemy::OnAttackNotify);
		AnimInstance->OnMontageEnded.AddDynamic(this, &AOPAG_ShieldEnemy::OnAttackEnd);
	}
}

void AOPAG_ShieldEnemy::Attack()
{
	AnimInstance->Montage_Play(ShieldAttackMontage, ShieldAttackMontagePlayRate /* = ?? */);
}

//Stops the enemy's ability to turn in the direction of the focus actor during the attack animation 
void AOPAG_ShieldEnemy::OnAttackBegin(UAnimMontage* Montage)
{
	bUseControllerRotationYaw = false;
}

//The attack collision & damage is computed only when the animation montage's notify is reached
void AOPAG_ShieldEnemy::OnAttackNotify(FName FunctionName, const FBranchingPointNotifyPayload& NotifyPayload)
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
				Player->ReceiveDamage(ShieldWeaponDamage);
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

//Resumes the enemy's ability to turn in the direction of the focus actor after the attack animation is done
void AOPAG_ShieldEnemy::OnAttackEnd(UAnimMontage* Montage, bool bInterrupted)
{
	bUseControllerRotationYaw = true;
}

void AOPAG_ShieldEnemy::Death()
{
	//Weapon & Shield Mesh ragdoll functions
	if(SwordMesh)
	{
		SwordMesh->K2_DetachFromComponent(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld);
		SwordMesh->SetSimulatePhysics(true);
		SwordMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	}
	if(ShieldMesh)
	{
		SwordMesh->K2_DetachFromComponent(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld);
		SwordMesh->SetSimulatePhysics(true);
		//TODO: Check if this is correct, given that the shield usually has collision unlike the sword
		SwordMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	}

	//Ragdoll stuff for the enemy mesh itself are done in the parent.
	Super::Death();
}
