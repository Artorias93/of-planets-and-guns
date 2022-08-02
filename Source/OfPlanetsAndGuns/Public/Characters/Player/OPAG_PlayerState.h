#pragma once
#include "Engine/DataTable.h"
#include "Weapons/Guns/OPAG_GunState.h"
#include "OPAG_PlayerState.generated.h"


USTRUCT(BlueprintType)
struct FOPAG_PlayerState : public FTableRowBase
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
		float Health = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
		int DashCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
		int GoldAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
		int BoltsAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
		int PotionsAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gun)
		FOPAG_GunState PrimaryGun;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gun)
		uint8 bIsPrimaryActive : 1;

};