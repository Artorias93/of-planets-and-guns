#pragma once

#include "CoreMinimal.h"
#include "Weapons/Modules/OPAG_AccessoryBase.h"
#include "OPAG_AccessoryDrone.generated.h"

class AOPAG_DroneBase;

UCLASS()
class OFPLANETSANDGUNS_API AOPAG_AccessoryDrone : public AOPAG_AccessoryBase
{
	GENERATED_BODY()

public:
	AOPAG_AccessoryDrone();

private:
	UPROPERTY(Category = "OPAG | Accessory | Drone", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	const TSubclassOf<AOPAG_DroneBase> DroneClass;

	UPROPERTY(Category = "OPAG | Accessory | Drone", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float TimeBeforeDestroy = 10.f;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	FORCEINLINE TSubclassOf<AOPAG_DroneBase> GetDroneClass() const { return DroneClass; }
	FORCEINLINE float GetTimeBeforeDestroyDrone() const { return TimeBeforeDestroy; }

};
