#pragma once

#include "CoreMinimal.h"
#include <Engine/DataTable.h>
#include "OPAG_ModuleStats.generated.h"

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

UENUM(BlueprintType)
enum class EModuleBrand : uint8
{
	Standard = 0x00,
	Kaponian,
	Uronian,
	Blobius,
	Neridian,
};

UENUM(BlueprintType)
enum class EModuleStatType : uint8
{
	Damage,
	FireRate,
	Accuracy,
	Stability,
	ReloadingTime,
	ClipSize
};

UENUM(BlueprintType)
enum class EAccessoryType : uint8
{
	None,
	Drone,
	Throwable,
	Shield
};

USTRUCT(BlueprintType)
struct OFPLANETSANDGUNS_API FOPAG_SecondaryStatModifier : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EModuleBrand Brand;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EModuleStatType StatType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MinValue;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MaxValue;
};

static int ModuleTypeCount()
{
	const FString enumName = "EModuleTypes";
	UEnum* enumObj = FindObject<UEnum>(ANY_PACKAGE, *enumName);
	int64 maxVal = enumObj->GetMaxEnumValue();

	return maxVal + 1;
}
