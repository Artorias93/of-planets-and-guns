#pragma once
#include "Engine/DataTable.h"
#include "OPAG_Settings.generated.h"

USTRUCT(BlueprintType)
struct OFPLANETSANDGUNS_API FOPAG_Settings : public FTableRowBase
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Aiming)
		float HipSensitivity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Aiming)
		float ADSSensitivity;
};