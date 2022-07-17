#pragma once

#include "CoreMinimal.h"
#include "Weapons/Modules/OPAG_ModuleBase.h"
#include "OPAG_AccessoryBase.generated.h"

class UGameplayAbility;
class AOPAG_Grenade;

UCLASS()
class OFPLANETSANDGUNS_API AOPAG_AccessoryBase : public AOPAG_ModuleBase
{
	GENERATED_BODY()
	
public:	
	AOPAG_AccessoryBase();

	UPROPERTY(Category = "Accessory Ability", EditDefaultsOnly, BlueprintReadWrite)
	const TSubclassOf<UGameplayAbility> Ability;

	UPROPERTY(Category = "Accessory Ability", EditAnywhere, BlueprintReadWrite)
	const TSubclassOf<AOPAG_Grenade> Grenade;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
