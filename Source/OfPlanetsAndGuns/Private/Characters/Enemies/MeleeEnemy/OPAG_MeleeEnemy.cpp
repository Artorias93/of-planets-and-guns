#include "Characters/Enemies/MeleeEnemy/OPAG_MeleeEnemy.h"
//My Classes
#include "Characters/Enemies/OPAG_AICBase.h"

AOPAG_MeleeEnemy::AOPAG_MeleeEnemy()
{
	PrimaryActorTick.bCanEverTick = false;

	SwordMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("SwordMeshComponent");
	SwordMeshComponent->SetupAttachment(GetMesh());
}

void AOPAG_MeleeEnemy::BeginPlay()
{
	Super::BeginPlay();
	SwordMeshComponent->AttachTo(GetMesh(), "RightHandSocket", EAttachLocation::KeepRelativeOffset, false);
	SwordMeshComponent->SetWorldScale3D(FVector(.2f));
}

void AOPAG_MeleeEnemy::Attack()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("I'm an attacking melee!"));

	if (AOPAG_AICBase* const AIController = Cast<AOPAG_AICBase>(GetController()))
	{
		AActor* const OtherActor = AIController->GetFocusActor();

		//TODO: Swap with proper player class when ready
		if(ACharacter* const PlayerCharacter = Cast<ACharacter>(OtherActor))
		{
			PlayMeleeAttackMontage();
		}
	}
}

float AOPAG_MeleeEnemy::PlayMeleeAttackMontage()
{
	UAnimInstance* AnimInstance = (GetMesh()) ? GetMesh()->GetAnimInstance() : nullptr;
	if (MeleeAttackMontage && AnimInstance)
	{
		float const Duration = AnimInstance->Montage_Play(MeleeAttackMontage, 1.9f);
		if (Duration > 0.f)
		{
			return Duration;
		}
	}
	return 0.f;
}