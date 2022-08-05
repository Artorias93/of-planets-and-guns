#pragma once


#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "OPAG_LootChance.generated.h"

UENUM(BlueprintType)
enum class ELootType : uint8
{
	Module,
	Gun,
	Drop
};

USTRUCT(BlueprintType)
struct OFPLANETSANDGUNS_API FOPAG_LootChance : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (MustImplement = "OPAG_InteractableInterface"))
		TSubclassOf<AActor> DropClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ELootType DropType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (UIMin = 0, UIMax = 100))
		float LootChance;
};