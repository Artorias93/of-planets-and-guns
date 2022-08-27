#include "Characters/Enemies/OPAG_Enemy.h"
#include <GameFramework/CharacterMovementComponent.h>
#include <BehaviorTree/BlackboardComponent.h>
#include <Perception/AISense_Damage.h>
#include <Kismet/GameplayStatics.h>
#include <BrainComponent.h>
#include <Curves/CurveVector.h>
#include <Kismet/KismetMathLibrary.h>
#include <Components/WidgetComponent.h>
#include <Components/ProgressBar.h>
//My Classes
#include "Characters/Enemies/OPAG_AICBase.h"
#include "Characters/OPAG_AttributeSetBase.h"
#include "Characters/Player/OPAG_Player.h"
#include "Managers/OPAG_EventManagerSubsystem.h"
#include "Managers/OPAG_GameInstance.h"
#include "Characters/Enemies/OPAG_LootDropSystemComponent.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "UI/OPAG_EnemyHealthBar.h"
#include "Characters/OPAG_CharacterBase.h"



AOPAG_Enemy::AOPAG_Enemy()
{
	PrimaryActorTick.bCanEverTick = true;

	HeadCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Head Collider"));
	HeadCollider->SetupAttachment(GetMesh(), TEXT("HeadCenter"));
	HeadCollider->SetRelativeScale3D(FVector::OneVector * 10.f);
	HeadCollider->ComponentTags.Add(TEXT("Head"));

	bIsJumping = false;

	LootDropSystemComponent = CreateDefaultSubobject<UOPAG_LootDropSystemComponent>(TEXT("Loot Drop System Component"));

	SpawnParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle System Component"));
	SpawnParticle->SetupAttachment(GetMesh());

	SpawnTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("SpawnTimeline"));

	const auto SpawnParticleFinder = ConstructorHelpers::FObjectFinder<UParticleSystem>(
		TEXT("ParticleSystem'/Game/OfPlanetsAndGuns/Effects/Teleportation/P_Teleportation.P_Teleportation'"));
	if (SpawnParticleFinder.Succeeded())
	{
		SpawnParticle->Template = SpawnParticleFinder.Object;
	}

	const auto SpawnMaterialFinder = ConstructorHelpers::FObjectFinder<UMaterialInstance>(
		TEXT("MaterialInstanceConstant'/Game/OfPlanetsAndGuns/Effects/Teleportation/body/MI_Orange.MI_Orange'"));
	if (SpawnMaterialFinder.Succeeded())
	{
		SpawnMaterial = SpawnMaterialFinder.Object;
	}

	const auto SpawnFXCurveFinder = ConstructorHelpers::FObjectFinder<UCurveVector>(
		TEXT("CurveVector'/Game/OfPlanetsAndGuns/Characters/Common/Curve_EnemySpawnFX_Vector.Curve_EnemySpawnFX_Vector'"));
	if (SpawnFXCurveFinder.Succeeded())
	{
		SpawnFXCurve = SpawnFXCurveFinder.Object;
	}

	EnemyHealthBarComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("EnemyHealthBar"));
	if (EnemyHealthBarComponent != nullptr) {
		EnemyHealthBarComponent->SetWorldLocation(FVector(0.f, 0.f, 150.f));
		EnemyHealthBarComponent->SetWidgetSpace(EWidgetSpace::World);
		EnemyHealthBarComponent->SetDrawSize(FVector2D(100.f, 10.f));
		EnemyHealthBarComponent->SetupAttachment(RootComponent);
	}


	BoosterParticleComponent1 = CreateDefaultSubobject<UParticleSystemComponent>("Booster Component 1");
	if(BoosterParticleComponent1)
	{
		BoosterParticleComponent1->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform,
			"LeftFootSocket");
	}
	
	BoosterParticleComponent2 = CreateDefaultSubobject<UParticleSystemComponent>("Booster Component 2");
	if(BoosterParticleComponent2)
	{
		BoosterParticleComponent2->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform,
			"RightFootSocket");
	}
}

void AOPAG_Enemy::BeginPlay()
{
	Super::BeginPlay();

	//Gets bUsingPatrolPath from the instance
	if (AOPAG_AICBase* const AIController = Cast<AOPAG_AICBase>(GetController()))
	{
		AIController->GetBlackboardComponent()->SetValueAsBool("bUsingPatrolPath", bUsingPatrolPath);
	}

	InitSpawnFX();

	if (EnemyHealthBarComponent != nullptr) {
		if (EnemyHealthBarComponent->GetWidgetClass() == nullptr)
			EnemyHealthBarComponent->SetWidgetClass(EnemyHealthBarClass);
		EnemyHealthBar = Cast<UOPAG_EnemyHealthBar>(EnemyHealthBarComponent->GetWidget());
		if (EnemyHealthBar == nullptr) return;
		const float HealthPercent = BaseAttributeSet->GetHealth() / BaseAttributeSet->GetMaxHealth();
		EnemyHealthBar->GetProgressBar()->SetPercent(HealthPercent);
		EnemyHealthBarComponent->SetHiddenInGame(true);
		//UE_LOG(LogTemp, Warning, TEXT("Health enemy: %f "), HealthPercent);
	}

}

void AOPAG_Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (EnemyHealthBarComponent != nullptr && !EnemyHealthBarComponent->bHiddenInGame) {
		const AOPAG_Player* PlayerRef = GetWorld()->GetFirstPlayerController()->GetPawn<AOPAG_Player>(); // player ref
		const FRotator LookToPlayerRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerRef->GetActorLocation());		
		EnemyHealthBarComponent->SetWorldRotation(LookToPlayerRotation);
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

	// Update health bar
	const float HealthPercent = BaseAttributeSet->GetHealth() / BaseAttributeSet->GetMaxHealth();
	EnemyHealthBar->GetProgressBar()->SetPercent(HealthPercent);
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

	// Set HealthBar hidden
	EnemyHealthBarComponent->SetHiddenInGame(true);

	//Mesh ragdoll functions
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetSimulatePhysics(true);
	GetWorld()->GetTimerManager().SetTimer(RagdollTimerHandle, this, &AOPAG_Enemy::DropAndDespawn, RagdollDurationSeconds);
}

void AOPAG_Enemy::DropAndDespawn()
{
	// Drop loot
	LootDropSystemComponent->DropLoot();

	//Fill with everything from drops to enemy manager stuff

	GetWorld()->GetTimerManager().ClearTimer(RagdollTimerHandle);
	//Broadcasts the kill to the event manager
	const auto GI = GetGameInstance<UOPAG_GameInstance>();
	if (GI) GI->GetEventManager()->EnemyKilled.Broadcast(this);
	Destroy();
}

bool AOPAG_Enemy::GetIsJumping() const
{
	return bIsJumping;
}

void AOPAG_Enemy::SetIsJumping(const bool Value)
{
	bIsJumping = Value;
}

void AOPAG_Enemy::TurnOnBoosters() const
{
	if(BoosterParticleComponent1)
	{
		BoosterParticleComponent1->SetVisibility(true);
	}
	if(BoosterParticleComponent2)
	{
		BoosterParticleComponent2->SetVisibility(true);
	}
}

void AOPAG_Enemy::TurnOffBoosters() const
{
	if(BoosterParticleComponent1)
	{
		BoosterParticleComponent1->SetVisibility(false);
	}
	if(BoosterParticleComponent2)
	{
		BoosterParticleComponent2->SetVisibility(false);
	}
}

void AOPAG_Enemy::InitSpawnFX()
{
	if (SpawnFXCurve)
	{
		ActiveMaterial = UMaterialInstanceDynamic::Create(SpawnMaterial, this);
		int i = 0;
		for (UMaterialInterface* MaterialInterface : GetMesh()->GetMaterials())
		{
			NormalMaterials.Add(MaterialInterface->GetMaterial());
			GetMesh()->SetMaterial(i, ActiveMaterial);
			++i;
		}

		SpawnTimeline->SetLooping(false);

		OnSpawnFXTimelineProgress.BindUFunction(this, FName("SpawnFXProgress"));
		SpawnTimeline->AddInterpVector(SpawnFXCurve, OnSpawnFXTimelineProgress);

		OnSpawnFXTimelineFinished.BindUFunction(this, FName("SpawnFXFinished"));
		SpawnTimeline->SetTimelineFinishedFunc(OnSpawnFXTimelineFinished);

		SpawnTimeline->PlayFromStart();
	}
}

void AOPAG_Enemy::SpawnFXProgress(FVector Value)
{
	ActiveMaterial->SetScalarParameterValue(TEXT("Amount"), Value.X);
	ActiveMaterial->SetScalarParameterValue(TEXT("Boost"), Value.Y);
}

void AOPAG_Enemy::SpawnFXFinished()
{
	for (int i = 0; i < NormalMaterials.Num(); ++i)
	{
		GetMesh()->SetMaterial(i, NormalMaterials[i]);
	}
	SpawnParticle->Deactivate();
	SpawnParticle->DestroyComponent();
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

void AOPAG_Enemy::SetHealthBarVisible()
{
	EnemyHealthBarComponent->SetHiddenInGame(false);
	GetWorld()->GetTimerManager().ClearTimer(UnseenByPlayerHandle);
	GetWorld()->GetTimerManager().SetTimer(UnseenByPlayerHandle, this, &AOPAG_Enemy::ResetHealthBarHidden, 3.f, false);
}

void AOPAG_Enemy::ResetHealthBarHidden()
{
	EnemyHealthBarComponent->SetHiddenInGame(true);
}



#pragma endregion