#include "Characters/Enemies/ShieldEnemy/OPAG_ShieldEnemy.h"
#include "Components/BoxComponent.h"
//My Classes
#include "NiagaraCommon.h"
#include "Characters/Enemies/OPAG_AICBase.h"
#include "Characters/Player/OPAG_Player.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

AOPAG_ShieldEnemy::AOPAG_ShieldEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	//When the melee walks it doesn't have to look at the player, or it slides on the ground
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;

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
		SwordMesh->AttachToComponent(GetMesh(),
			FAttachmentTransformRules::KeepRelativeTransform, "RightHandSocket");
	}

	ShieldMesh = CreateDefaultSubobject<UStaticMeshComponent>("Shield Mesh Component");
	if (ShieldMesh)
	{
		ShieldMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform,
			"LeftHandSocket");
		ShieldMesh->SetCanEverAffectNavigation(false);
		ShieldMesh->ComponentTags.Add(FName("Shield"));
	}

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AOPAG_ShieldEnemy::OnOverlapBegin);
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
		//AnimInstance->OnMontageEnded.AddDynamic(this, &AOPAG_ShieldEnemy::OnAttackEnd);
	}

	bIsCharging = false;
}

void AOPAG_ShieldEnemy::Attack()
{
	AnimInstance->Montage_Play(ShieldAttackMontage, ShieldAttackMontagePlayRate /* = ?? */);
}

//Stops the enemy's ability to turn in the direction of the focus actor during the attack animation 
void AOPAG_ShieldEnemy::OnAttackBegin(UAnimMontage* Montage)
{
	//Locks the horizontal rotation for the duration of the attack animation
	//bUseControllerRotationYaw = false;
	//Locks the Y axis of the rotation so that the enemy is never rotated upwards or downwards while attacking
	SetActorRotation(FRotator(0.f, GetActorRotation().Yaw, GetActorRotation().Roll));
}

//Resumes the enemy's ability to turn in the direction of the focus actor after the attack animation is done
void AOPAG_ShieldEnemy::OnAttackEnd(UAnimMontage* Montage, bool bInterrupted)
{
	//Resumes the horizontal rotation following the controller's
	//bUseControllerRotationYaw = true;
}

//The attack collision & damage is computed only when the animation montage's notify is reached
void AOPAG_ShieldEnemy::OnAttackNotify(FName FunctionName, const FBranchingPointNotifyPayload& NotifyPayload)
{
	if (const AOPAG_AICBase* const AIController = Cast<AOPAG_AICBase>(GetController()))
	{
		if (DamageCollisionBox->IsOverlappingActor(AIController->GetFocusActor()))
		{
			if(AOPAG_Player* const Player = Cast<AOPAG_Player>(AIController->GetFocusActor()))
			{
				Player->ReceiveDamage(ShieldWeaponDamage, this);
			}
		}
	}	
}

void AOPAG_ShieldEnemy::Death()
{
	GetCharacterMovement()->GravityScale = 1.f;
	//Weapon & Shield Mesh ragdoll functions
	if(SwordMesh)
	{
		SwordMesh->K2_DetachFromComponent(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld);
		SwordMesh->SetSimulatePhysics(true);
		SwordMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	}
	if(ShieldMesh)
	{
		ShieldMesh->K2_DetachFromComponent(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld);
		ShieldMesh->SetSimulatePhysics(true);
		ShieldMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	}

	//Ragdoll stuff for the enemy mesh itself are done in the parent.
	Super::Death();
}

void AOPAG_ShieldEnemy::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(AOPAG_Player* const OverlapPlayer = Cast<AOPAG_Player>(OtherActor))
	{
		OverlapPlayer->ReceiveDamage(ChargeDamage, this);
	}
}

bool AOPAG_ShieldEnemy::GetIsCharging() const
{
	return bIsCharging;
}

void AOPAG_ShieldEnemy::SetIsCharging(const bool Value)
{
	bIsCharging = Value;
}

UStaticMeshComponent* AOPAG_ShieldEnemy::GetShieldMesh() const
{
	return ShieldMesh;
}