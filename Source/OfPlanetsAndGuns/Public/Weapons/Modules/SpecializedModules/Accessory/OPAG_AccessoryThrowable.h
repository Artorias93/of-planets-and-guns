#pragma once

#include "CoreMinimal.h"
#include "Weapons/Modules/OPAG_AccessoryBase.h"
#include "OPAG_AccessoryThrowable.generated.h"

class AOPAG_Throwable;

UCLASS()
class OFPLANETSANDGUNS_API AOPAG_AccessoryThrowable : public AOPAG_AccessoryBase
{
	GENERATED_BODY()
	
	UPROPERTY(Category = "Accessory Ability", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	const TSubclassOf<AOPAG_Throwable> Throwable;

public:
	AOPAG_AccessoryThrowable();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	TSubclassOf<AOPAG_Throwable> GetThrowable() const;

};
