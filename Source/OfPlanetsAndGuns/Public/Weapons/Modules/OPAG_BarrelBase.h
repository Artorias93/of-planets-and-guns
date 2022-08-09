#pragma once

#include "CoreMinimal.h"
#include "Weapons/Modules/OPAG_ModuleBase.h"
#include "OPAG_BarrelBase.generated.h"

class UChildActorComponent;

UCLASS()
class OFPLANETSANDGUNS_API AOPAG_BarrelBase : public AOPAG_ModuleBase
{
	GENERATED_BODY()
	
public:
	AOPAG_BarrelBase();

	UPROPERTY(Category = "Module", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* MuzzleComponent;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
