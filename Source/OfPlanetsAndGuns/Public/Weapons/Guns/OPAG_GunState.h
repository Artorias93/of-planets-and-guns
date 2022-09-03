#pragma once

#include "CoreMinimal.h"
#include <Engine/DataTable.h>

#include "OPAG_GunStats.h"
#include "OPAG_GunState.generated.h"

class AOPAG_ModuleBase;
class AOPAG_GunBase;


USTRUCT(BlueprintType)
struct FOPAG_GunModules : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AOPAG_ModuleBase> Barrel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AOPAG_ModuleBase> Stock;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AOPAG_ModuleBase> Grip;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AOPAG_ModuleBase> Magazine;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AOPAG_ModuleBase> Sight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AOPAG_ModuleBase> Accessory;
};

USTRUCT(BlueprintType)
struct FOPAG_GunModuleStats
{

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FOPAG_GunStats BarrelStats;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FOPAG_GunStats StockStats;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FOPAG_GunStats GripStats;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FOPAG_GunStats MagazineStats;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FOPAG_GunStats SightStats;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FOPAG_GunStats AccessoryStats;
};


USTRUCT(BlueprintType)
struct FOPAG_GunState : public FTableRowBase
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AOPAG_GunBase> GunClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FOPAG_GunModules AttachedModules;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FOPAG_GunModuleStats AttachedModuleStats;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int AmmoCurrent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int AmmoMax;

};