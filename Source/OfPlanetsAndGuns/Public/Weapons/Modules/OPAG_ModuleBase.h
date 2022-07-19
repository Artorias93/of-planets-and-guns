#pragma once

#include "Weapons/Guns/OPAG_GunBase.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OPAG_ModuleBase.generated.h"

class USphereComponent;
class UStaticMeshComponent;

UENUM(BlueprintType)
enum class EModuleTypes : uint8
{
	Barrel = 0x00,
	Stock,
	Grip,
	Magazine,
	Sight,
	Accessory,
};

UENUM(BlueprintType)
enum class EModuleRarity : uint8
{
	Common = 0x00,
	Uncommon,
	Rare,
	Epic,
	Legendary,
};

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
	FGunStats Stats;

	UPROPERTY(Category = "Module", EditAnywhere, BlueprintReadWrite, meta = (Bitmask, BitmaskEnum = "EFireTypes"))
	int EquippableFireTypes;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void OnAttach(const EModuleRarity NewRarity, const FGunStats NewStats, const int NewEquippableFireTypes);
	void SetParameters(const EModuleRarity NewRarity, const FGunStats NewStats, const int NewEquippableFireTypes);
	UPrimitiveComponent* GetSphereComponent() const;

private:
	void CheckIfPhysical();
};