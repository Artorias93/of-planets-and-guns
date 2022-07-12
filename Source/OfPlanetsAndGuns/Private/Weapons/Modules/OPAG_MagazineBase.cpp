#include "Weapons/Modules/OPAG_MagazineBase.h"

AOPAG_MagazineBase::AOPAG_MagazineBase() : Super::AOPAG_ModuleBase()
{
	// Initialize Parameters
	Rarity = EModuleRarity::Common;
	Type = EModuleTypes::Magazine;
}

void AOPAG_MagazineBase::BeginPlay()
{
	Super::BeginPlay();

}

void AOPAG_MagazineBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}