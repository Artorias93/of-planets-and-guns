// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/OPAG_Drop.h"
#include "OPAG_Ammo.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EAmmoType : uint8 {
	FullyAutomatic,
	SemiAutomatic,
	Shotgun,
	Sniper,
	SMG
};

UCLASS()
class OFPLANETSANDGUNS_API AOPAG_Ammo : public AOPAG_Drop
{
	GENERATED_BODY()

	AOPAG_Ammo();
};
