#pragma once

#include "Weapons/Guns/OPAG_GunBase.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OPAG_ModuleBase.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class AOPAG_GunBase;

UENUM(BlueprintType)
enum EModuleTypes 
{
	Barrel,
	Stock,
	Grip,
	Magazine,
	Sight,
	Accessory,
};

UENUM(BlueprintType)
enum EModuleRarity
{
	Common,
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

	UPROPERTY(Category = "Module", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USphereComponent* SphereComponent;

	UPROPERTY(Category = "Module", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh;

public:
	UPROPERTY(Category = "Module", VisibleAnywhere, BlueprintReadWrite)
	TEnumAsByte<EModuleTypes> Type;

	UPROPERTY(Category = "Module", EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EModuleRarity> Rarity;

	UPROPERTY(Category = "Module", EditAnywhere, BlueprintReadWrite)
	FGunStats Stats;

	UPROPERTY(Category = "Module", EditAnywhere, BlueprintReadWrite)
	EFireTypes EquippableFireTypes;

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	virtual void OnAttach(EModuleRarity NewRarity);

public:	
	virtual void Tick(float DeltaTime) override;

};