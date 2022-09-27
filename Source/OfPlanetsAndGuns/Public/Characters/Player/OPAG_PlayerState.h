#pragma once
#include "Engine/DataTable.h"
#include "Weapons/Guns/OPAG_GunState.h"
#include "OPAG_PlayerState.generated.h"

UENUM(BlueprintType)
enum class PlayerStateLoadType : uint8
{
	NOT_LOADED,
	SAVE_FILE,
	PREVIOUS_STAGE
};

USTRUCT(BlueprintType)
struct OFPLANETSANDGUNS_API FOPAG_PlayerState : public FTableRowBase
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LoadType)
		PlayerStateLoadType LoadType = PlayerStateLoadType::NOT_LOADED;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
		float Health = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
		float Shield;

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