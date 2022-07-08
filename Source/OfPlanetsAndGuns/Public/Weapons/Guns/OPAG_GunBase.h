#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OPAG_GunBase.generated.h"

UENUM(BlueprintType, meta = (Bitflags))
enum class EFireTypes : uint8
{
	FullyAutomatic = 0x00,
	SingleShot = 0x01,
	Burst3 = 0x02,
	Shotgun = 0x03,
};
ENUM_CLASS_FLAGS(EFireTypes);

USTRUCT(BlueprintType)
struct FGunStats
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY()
	float Damage = 0;

	UPROPERTY()
	float FireRate = 0;

	UPROPERTY()
	float Accuracy = 0;

	UPROPERTY()
	float RecoilRate = 0;

	UPROPERTY()
	float ReloadingTime = 0;

	UPROPERTY()
	int ClipSize = 0;
};

UCLASS()
class OFPLANETSANDGUNS_API AOPAG_GunBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AOPAG_GunBase();

public:
	UPROPERTY(Category = "Gun", EditDefaultsOnly, BlueprintReadWrite)
	EFireTypes Type;

	UPROPERTY(Category = "Gun", EditAnywhere, BlueprintReadWrite)
	FGunStats Stats;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};