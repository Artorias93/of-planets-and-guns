#pragma once

#include "CoreMinimal.h"
#include "Weapons/Modules/OPAG_ModuleBase.h"
#include "OPAG_SightBase.generated.h"

class UCameraComponent;

UCLASS()
class OFPLANETSANDGUNS_API AOPAG_SightBase : public AOPAG_ModuleBase
{
	GENERATED_BODY()
	
	UPROPERTY(Category = Camera, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComponent;

public:
	AOPAG_SightBase();

protected:
	virtual void BeginPlay() override;
	virtual void OnAttach(const EModuleRarity NewRarity, const FGunStats NewStats, const int NewEquippableFireTypes) override;

public:
	virtual void Tick(float DeltaTime) override;
	UCameraComponent* GetCamera() const;
};
