#include "Weapons/Modules/OPAG_AccessoryBase.h"
#include "Abilities/GameplayAbility.h"
#include "Weapons/Throwables/OPAG_Grenade.h"

AOPAG_AccessoryBase::AOPAG_AccessoryBase()
{
 	PrimaryActorTick.bCanEverTick = true;

	// Initialize Parameters
	Rarity = EModuleRarity::Common;
	Type = EModuleTypes::Accessory;
}

void AOPAG_AccessoryBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AOPAG_AccessoryBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}