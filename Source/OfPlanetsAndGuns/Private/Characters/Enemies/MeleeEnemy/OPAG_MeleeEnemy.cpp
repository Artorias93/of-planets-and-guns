#include "Characters/Enemies/MeleeEnemy/OPAG_MeleeEnemy.h"
#include <Components/BoxComponent.h>
//My Classes
#include "Characters/Enemies/OPAG_AICBase.h"

AOPAG_MeleeEnemy::AOPAG_MeleeEnemy()
{
	PrimaryActorTick.bCanEverTick = false;

	//Components creation
	DamageCollisionBoxComponent = CreateDefaultSubobject<UBoxComponent>("Damage Collision Box");
	DamageCollisionBoxComponent->SetupAttachment(RootComponent);

	//TODO: Is there a smart way to do this in C++ (especially setting relative location and rotation)?
	SwordMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("SwordMeshComponent");
}

void AOPAG_MeleeEnemy::BeginPlay()
{
	Super::BeginPlay();

	DamageCollisionBoxComponent->SetRelativeLocation(DamageCollisionBoxComponentRelativeLocation);
	DamageCollisionBoxComponent->SetBoxExtent(DamageCollisionBoxComponentExtent);

	if (SwordMeshComponent)
	{
		SwordMeshComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "RightHandSocket");
	}

	//Sets delegate functions that enable or disable the pawn rotation at the start and end of the attack animation.
	//There's a notify in the middle of the animation that is when the collision checks and the damage should be evaluated.
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
	AnimInstance->Montage_Play(MeleeAttackMontage, 1.9f); //Montage speed = 1.9
}

void AOPAG_MeleeEnemy::OnAttackBegin(UAnimMontage* Montage)
{
	bUseControllerRotationYaw = false;
}

void AOPAG_MeleeEnemy::OnAttackEnd(UAnimMontage* Montage, bool Idontknow)
{
	bUseControllerRotationYaw = true;
}

void AOPAG_MeleeEnemy::OnAttackNotify(FName FunctionName, const FBranchingPointNotifyPayload &NotifyPayload)
{
	if (AOPAG_AICBase* const AIController = Cast<AOPAG_AICBase>(GetController()))
	{
		if (DamageCollisionBoxComponent->IsOverlappingActor(AIController->GetFocusActor()))
		{
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("HIT!"));

			//TODO: Check when player is done
			if(AOPAG_CharacterBase* Act = Cast<AOPAG_CharacterBase>(AIController->GetFocusActor()))
			{
				Act->TakeDamage(50.f);
			}

			//TODO: Not working
			//InflictDamage(50.f);

		}
		else
		{
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("MISS..."));
		}
	}
}