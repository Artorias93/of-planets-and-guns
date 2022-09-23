#include "Weapons/Modules/OPAG_MagazineBase.h"

AOPAG_MagazineBase::AOPAG_MagazineBase()
{
	// Initialize Parameters
	Brand = EModuleBrand::Standard;
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