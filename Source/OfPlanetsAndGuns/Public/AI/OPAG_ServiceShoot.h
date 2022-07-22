#pragma once

#include "CoreMinimal.h"
#include "OPAG_ServiceBase.h"
#include <NiagaraComponent.h>
#include "OPAG_ServiceShoot.generated.h"

UCLASS()
class OFPLANETSANDGUNS_API UOPAG_ServiceShoot : public UOPAG_ServiceBase
{
	GENERATED_BODY()

	/////////////////////////////
	//Functions
	/////////////////////////////

public:
	UOPAG_ServiceShoot(FObjectInitializer const& ObjectInitializer);

	virtual void TickNode(UBehaviorTreeComponent& OwnerBTComponent, uint8* NodeMemory, float DeltaSeconds) override;

	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerBTComponent, uint8* NodeMemory) override;

private:

	void DisableAimingSignals();

	/////////////////////////////
	//Variables
	/////////////////////////////

private:

	bool bIsAiming = false;
	
	UPROPERTY(EditAnywhere, Category = "Values")
	float StartShootingTimer = 3.f;

	FTimerHandle StartShootingTimerHandle;

	UPROPERTY()
	UStaticMeshComponent* AimingLed;

	UPROPERTY()
	UNiagaraComponent* LaserBeam;
};
