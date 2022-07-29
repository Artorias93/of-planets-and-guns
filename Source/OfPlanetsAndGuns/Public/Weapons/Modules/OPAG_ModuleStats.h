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