#pragma once


#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "OPAG_LootChance.generated.h"

class AOPAG_Drop;
class AOPAG_ModularGun;
class AOPAG_ModuleBase;

UENUM(BlueprintType)
enum class ELootType : uint8
{
	Gold,
	Ammo,
	Bolts,
	Potion,
	Module,
	Gun
};

USTRUCT(BlueprintType)
struct OFPLANETSANDGUNS_API FOPAG_LootChance : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ELootType LootType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (UIMin = 0, UIMax = 100))
	float LootChance;
};

USTRUCT(BlueprintType)
struct OFPLANETSANDGUNS_API FOPAG_DropList : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ELootType DropType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AOPAG_Drop> DropClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int DropAmount;
};

USTRUCT(BlueprintType)
struct OFPLANETSANDGUNS_API FOPAG_ModuleList : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AOPAG_ModuleBase> ModuleClass;
};

USTRUCT(BlueprintType)
struct OFPLANETSANDGUNS_API FOPAG_GunList : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AOPAG_ModularGun> GunClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (UIMin = 0, UIMax = 100))
	float GunChance;
};

/*UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (MustImplement = "OPAG_InteractableInterface"))
		TSubclassOf<AActor> DropClass;*/