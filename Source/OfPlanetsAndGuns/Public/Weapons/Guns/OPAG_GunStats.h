#pragma once

#include "CoreMinimal.h"
#include <Engine/DataTable.h>
#include "OPAG_GunStats.generated.h"

UENUM(BlueprintType, meta = (Bitflags))
enum class EFireTypes : uint8
{
	FullyAutomatic = 0x00,
	SingleShot = 0x01,
	Burst3 = 0x02,
	Shotgun = 0x03,
	Pistol = 0x04,
};
ENUM_CLASS_FLAGS(EFireTypes);

USTRUCT(BlueprintType)
struct OFPLANETSANDGUNS_API FOPAG_ReloadStats : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EFireTypes FireType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int ReloadingFactor;

};

USTRUCT(BlueprintType)
struct OFPLANETSANDGUNS_API FOPAG_GunStats : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float FireRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0.f, ClampMax = 100.f))
		float Accuracy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float RecoilRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ReloadingTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int ClipSize;

	FOPAG_GunStats() : Damage(0), FireRate(0), Accuracy(0), RecoilRate(0), ReloadingTime(0), ClipSize(0) {}
};