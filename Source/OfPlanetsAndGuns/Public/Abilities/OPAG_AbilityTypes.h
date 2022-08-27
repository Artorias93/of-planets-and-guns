#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Characters/Enemies/OPAG_Enemy.h"
#include "OPAG_AbilityTypes.generated.h"

UENUM(BlueprintType)
enum class EAbilityTypes : uint8
{
	ForceField,
	Tornado,
	Crack,
	Ray,
	Supernova,
	Fireball,
	Spawn_ShieldEnemy,
	Spawn_RangedEnemy,
	Spawn_MachineGunEnemy,
	Spawn_SniperEnemy,
	Spawn_MeleeEnemy,
	Spawn_PillarEnemy,
	Spawn_DashingEnemy
};

UENUM()
enum class EAbilitySpawnPattern : uint8
{
	Simple,
	SingleFrontLeftCorner,
	SingleFrontRightCorner,
	SingleBackLeftCorner,
	SingleBackRightCorner,
	GroundedFrontSingle,
	GroundedLineFrontTwo,
	GroundedLineFrontThree,
	GroundedLineBackSingle,
	GroundedLineBackTwo,
	GroundedLineBackThree,
	GroundedTridentBackThree,
	GroundedTridentFrontThree,
	CenteredStarFour,
	CenteredFrontSingle,
	CenteredUpSingle,
	CenteredUpSequenceStar,
};

USTRUCT(BlueprintType)
struct OFPLANETSANDGUNS_API FOPAG_AbilityTypeToEnemyMapping : public FTableRowBase
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AOPAG_Enemy> EnemyClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EAbilityTypes AbilityType;
};