#include "AI/OPAG_ServiceShoot.h"
#include <Kismet/KismetSystemLibrary.h>
#include "GameFramework/Actor.h"
//My Classes
#include "Characters/Enemies/OPAG_AICBase.h"
#include "Characters/Enemies/RangedEnemy/OPAG_RangedEnemy.h"
#include "Characters/Enemies/NormalTurret/OPAG_NormalTurret.h"
#include "Characters/Player/OPAG_Player.h"

UOPAG_ServiceShoot::UOPAG_ServiceShoot(FObjectInitializer const& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = TEXT("Shoot");
	bNotifyTick = true;
	bNotifyBecomeRelevant = true;
	bCreateNodeInstance = true; //This way it create separate instances for the service for different objects, and the timer works as intended
}

//Every service tick it checks if there's a SphereTrace between the RangedEnemy and the player.
//If there's one it calls the RangedEnemy's Attack function
void UOPAG_ServiceShoot::TickNode(UBehaviorTreeComponent& OwnerBTComponent, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerBTComponent, NodeMemory, DeltaSeconds);

	if(!bIsAiming)
	{
		if (const AOPAG_AICBase* const AIController = Cast<AOPAG_AICBase>(OwnerBTComponent.GetAIOwner()))
		{
			if (const AOPAG_Player* const FocusActor = Cast<AOPAG_Player>(AIController->GetFocusActor()))
			{
				if(AOPAG_Enemy* const Enemy = Cast<AOPAG_Enemy>(AIController->GetPawn()))
				{
					//Constructing what's needed for the LineTrace
					TArray<AActor*> ActorsToIgnore;
					ActorsToIgnore.Add(Enemy);
					FHitResult HitResult;
					const bool TraceResult = UKismetSystemLibrary::LineTraceSingle(GetWorld(),
						Enemy->GetActorLocation(), FocusActor->GetActorLocation(),
						TraceTypeQuery2 /* = Camera*/, false, ActorsToIgnore,
						EDrawDebugTrace::None, HitResult, true);

					if (TraceResult)
					{
						Enemy->Attack();
					}
				}
			}
		}
	}
}

//Executed on service activation
void UOPAG_ServiceShoot::OnBecomeRelevant(UBehaviorTreeComponent& OwnerBTComponent, uint8* NodeMemory)
{
	if(const AOPAG_AICBase* const AIController = Cast<AOPAG_AICBase>(OwnerBTComponent.GetAIOwner()))
	{
		if(AOPAG_RangedEnemy* const RangedEnemy = Cast<AOPAG_RangedEnemy>(AIController->GetPawn()))
		{
			bIsAiming = true;

			AimingLed = Cast<UStaticMeshComponent>(RangedEnemy->GetAimingLed());
			if(AimingLed)
			{
				AimingLed->SetVisibility(true);
			}
			
			if(!RangedEnemy->GetHasShotSinceSpawning())
			{
				LaserBeam = Cast<UNiagaraComponent>(RangedEnemy->GetLaserBeam());
				if (LaserBeam)
				{
					LaserBeam->SetVisibility(true);
					RangedEnemy->SetHasShotSinceSpawning(true);
				}
			}
			GetWorld()->GetTimerManager().SetTimer(StartShootingTimerHandle, this, &UOPAG_ServiceShoot::DisableAimingSignals, StartShootingTimer, false);
		}

		if (AOPAG_NormalTurret* const Turret = Cast<AOPAG_NormalTurret>(AIController->GetPawn()))
		{
			bIsAiming = true;		

			LaserBeam = Cast<UNiagaraComponent>(Turret->GetLaserBeam());
			if (LaserBeam)
			{
				LaserBeam->SetVisibility(true);				
			}			

			GetWorld()->GetTimerManager().SetTimer(StartShootingTimerHandle, this, &UOPAG_ServiceShoot::TurretStartShooting, StartShootingTimer, false);
		}
	}
}

void UOPAG_ServiceShoot::DisableAimingSignals()
{
	bIsAiming = false;
	AimingLed->SetVisibility(false);
	LaserBeam->SetVisibility(false);
}

void UOPAG_ServiceShoot::TurretStartShooting()
{
	bIsAiming = false;
	LaserBeam->SetVisibility(false);
}