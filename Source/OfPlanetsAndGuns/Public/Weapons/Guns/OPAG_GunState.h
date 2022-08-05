#pragma once

#include "CoreMinimal.h"
#include <Engine/DataTable.h>
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
struct FOPAG_GunState : public FTableRowBase
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AOPAG_GunBase> GunClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FOPAG_GunModules AttachedModules;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int AmmoCurrent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int AmmoMax;

};