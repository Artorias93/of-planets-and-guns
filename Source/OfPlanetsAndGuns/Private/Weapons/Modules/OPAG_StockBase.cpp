#include "Weapons/Modules/OPAG_StockBase.h"

AOPAG_StockBase::AOPAG_StockBase()
{
	// Initialize Parameters
	Rarity = EModuleRarity::Common;
	Type = EModuleTypes::Stock;
}

void AOPAG_StockBase::BeginPlay()
{
	Super::BeginPlay();

}

void AOPAG_StockBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}