#pragma once

#include "Weapons/Guns/OPAG_GunBase.h"
#include "CoreMinimal.h"
#include "OPAG_ModuleStats.h"
#include "GameFramework/Actor.h"
#include "OPAG_ModuleBase.generated.h"

class USphereComponent;
class UStaticMeshComponent;



UCLASS()
class OFPLANETSANDGUNS_API AOPAG_ModuleBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AOPAG_ModuleBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USphereComponent* Sphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ModuleMesh;

public:
	UPROPERTY(Category = "Module", EditAnywhere, BlueprintReadWrite)
	EModuleTypes Type;

	UPROPERTY(Category = "Module", EditAnywhere, BlueprintReadWrite)
	EModuleRarity Rarity;

	UPROPERTY(Category = "Module", EditAnywhere, BlueprintReadWrite)
	FOPAG_GunStats Stats;

	UPROPERTY(Category = "Module", EditAnywhere, BlueprintReadWrite, meta = (Bitmask, BitmaskEnum = "EFireTypes"))
	int EquippableFireTypes;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void OnAttach(const EModuleRarity NewRarity, const FOPAG_GunStats NewStats, const int NewEquippableFireTypes);
	void SetParameters(const EModuleRarity NewRarity, const FOPAG_GunStats NewStats, const int NewEquippableFireTypes);
	UPrimitiveComponent* GetSphereComponent() const;

private:
	void CheckIfPhysical();
};