#pragma once

#include "CoreMinimal.h"
#include "Weapons/Modules/OPAG_ModuleBase.h"
#include "OPAG_MagazineBase.generated.h"

UCLASS()
class OFPLANETSANDGUNS_API AOPAG_MagazineBase : public AOPAG_ModuleBase
{
	GENERATED_BODY()

public:
	AOPAG_MagazineBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

};
