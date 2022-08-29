#pragma once

#include "CoreMinimal.h"
#include "Weapons/Guns/OPAG_ModularGun.h"
#include "OPAG_Shotgun.generated.h"

UCLASS()
class OFPLANETSANDGUNS_API AOPAG_Shotgun final : public AOPAG_ModularGun
{
	GENERATED_BODY()
	
public:
	AOPAG_Shotgun();

	UPROPERTY(Category = "OPAG | Gun | Shooting", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", UIMin = 10, UIMax = 30))
	int ConsecutiveProjectile = 15;

private:
	virtual void ShootLineTrace(FVector StartShootLocation, FVector Direction, const AOPAG_CharacterBase* ShooterCharacter) override;
};
