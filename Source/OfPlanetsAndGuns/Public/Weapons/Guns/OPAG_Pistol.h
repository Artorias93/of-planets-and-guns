#pragma once

#include "CoreMinimal.h"
#include "Weapons/Guns/OPAG_GunBase.h"
#include "OPAG_Pistol.generated.h"

class UKismetSystemLibrary;
class AOPAG_CharacterBase;

UCLASS()
class OFPLANETSANDGUNS_API AOPAG_Pistol : public AOPAG_GunBase
{
	GENERATED_BODY()
	
public:
	AOPAG_Pistol();

	UPROPERTY(Category = "Pistol", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		USceneComponent* MuzzleComponent;

protected:
	virtual void BeginPlay() override;
	virtual USceneComponent* GetMuzzle() const override;

public:
	virtual void Tick(float DeltaTime) override;

};
