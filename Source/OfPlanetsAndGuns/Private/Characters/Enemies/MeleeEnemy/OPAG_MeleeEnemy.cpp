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

	//Sets a delegate function that processes the attack only when the animation notify is reached
	AnimInstance = (GetMesh()) ? GetMesh()->GetAnimInstance() : nullptr;
	if (MeleeAttackMontage && AnimInstance)
	{
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &AOPAG_MeleeEnemy::OnAttackNotify);
	}

	
	//SwordMeshComponent->SetRelativeLocation(FVector(21.6923f, -18.8085f, 0.9923f));
	//SwordMeshComponent->SetRelativeRotation(FRotator(10.9306f, 49.441f, -107.246f));
}

void AOPAG_MeleeEnemy::Attack()
{
	AnimInstance->Montage_Play(MeleeAttackMontage, 1.9f); //Montage speed = 1.9
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