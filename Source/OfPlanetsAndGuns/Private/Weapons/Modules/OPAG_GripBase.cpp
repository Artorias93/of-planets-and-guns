#include "Weapons/Modules/OPAG_GripBase.h"

AOPAG_GripBase::AOPAG_GripBase()
{
	// Initialize Parameters
	Rarity = EModuleRarity::Common;
	Type = EModuleTypes::Grip;
}

void AOPAG_GripBase::BeginPlay()
{
	Super::BeginPlay();

}

void AOPAG_GripBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}