#pragma once

#include "CoreMinimal.h"
#include "Weapons/Guns/OPAG_ModularGun.h"
#include "OPAG_Shotgun.generated.h"

UCLASS()
class OFPLANETSANDGUNS_API AOPAG_Shotgun : public AOPAG_ModularGun
{
	GENERATED_BODY()
	
public:
	AOPAG_Shotgun();

	UPROPERTY(Category = "Guns", EditAnywhere, BlueprintReadWrite)
	int ConsecutiveProjectile = 15;

protected:
	virtual void ShootLineTrace(FVector StartShootLocation, FVector Direction, const AOPAG_CharacterBase* ShooterCharacter) override;
};
