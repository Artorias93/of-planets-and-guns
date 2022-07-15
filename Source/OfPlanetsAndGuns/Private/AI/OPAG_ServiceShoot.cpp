#include "AI/OPAG_ServiceShoot.h"
#include <Kismet/KismetSystemLibrary.h>
//My Classes
#include "Characters/Enemies/RangedEnemy/OPAG_AICRangedEnemy.h"
#include "Characters/Enemies/RangedEnemy/OPAG_RangedEnemy.h"

UOPAG_ServiceShoot::UOPAG_ServiceShoot(FObjectInitializer const& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = TEXT("Shoot");
	bNotifyTick = true;
	bNotifyBecomeRelevant = true;
}

//Every service tick it checks if there's a SphereTrace between the RangedEnemy and the player.
//If there's one it calls the RangedEnemy's Attack function
void UOPAG_ServiceShoot::TickNode(UBehaviorTreeComponent& OwnerBTComponent, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerBTComponent, NodeMemory, DeltaSeconds);

	if(bIsAiming)
	{
		if (AOPAG_AICRangedEnemy* const AIController = Cast<AOPAG_AICRangedEnemy>(OwnerBTComponent.GetAIOwner()))
		{
			//TODO: Replace with player when ready
			if (AOPAG_CharacterBase* const FocusActor = Cast<AOPAG_CharacterBase>(AIController->GetFocusActor()))
			{
				if(AOPAG_RangedEnemy* const RangedEnemy = Cast<AOPAG_RangedEnemy>(AIController->GetPawn()))
				{
					//Constructing what's needed for the SphereTrace
					TArray<AActor*> ActorsToIgnore;
					ActorsToIgnore.Add(RangedEnemy);
					FHitResult HitResult;
					//TODO: Why is this a Sphere Trace with radius 0 and not a Line Trace?
					bool TraceResult = UKismetSystemLibrary::SphereTraceSingle(GetWorld(), RangedEnemy->GetActorLocation(), FocusActor->GetActorLocation(), 0.f, TraceTypeQuery2 /* = Camera*/, false, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true);

					if (TraceResult)
					{
						RangedEnemy->Attack();
					}
				}
			}
		}
	}
}


//On activation???
void UOPAG_ServiceShoot::OnBecomeRelevant(UBehaviorTreeComponent& OwnerBTComponent, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerBTComponent, NodeMemory);

	if(AOPAG_AICRangedEnemy* const AIController = Cast<AOPAG_AICRangedEnemy>(OwnerBTComponent.GetAIOwner()))
	{
		if(AOPAG_RangedEnemy* const RangedEnemy = Cast<AOPAG_RangedEnemy>(AIController->GetPawn()))
		{
			bIsAiming = true;

			AimingLed = Cast<UStaticMeshComponent>(RangedEnemy->AimingLed);
			if(AimingLed)
			{
				AimingLed->SetVisibility(true);
			}

			LaserBeam = Cast<UNiagaraComponent>(RangedEnemy->LaserBeam);
			if (LaserBeam)
			{
				//What is this????
				if (/*!RangedEnemy->Shoot*/ true)
				{
					LaserBeam->SetVisibility(true);
					//RangedEnemy->Shoot = true;
				}
			}

			GetWorld()->GetTimerManager().SetTimer(StartShootingTimerHandle, this, &UOPAG_ServiceShoot::DisableAimingSignals, StartShootingTimer, false);
		}
	}
}

void UOPAG_ServiceShoot::DisableAimingSignals()
{
	bIsAiming = false;
	AimingLed->SetVisibility(false);
	LaserBeam->SetVisibility(false);
}