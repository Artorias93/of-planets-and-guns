#pragma once

#include "CoreMinimal.h"
#include "Weapons/Modules/OPAG_ModuleBase.h"
#include "OPAG_GripBase.generated.h"

UCLASS()
class OFPLANETSANDGUNS_API AOPAG_GripBase : public AOPAG_ModuleBase
{
	GENERATED_BODY()

public:
	AOPAG_GripBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

};
