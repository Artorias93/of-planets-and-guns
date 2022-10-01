// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OPAG_ModuleStatItem.h"

UOPAG_ModuleStatItem::UOPAG_ModuleStatItem()
{
	//Debug purposes
	Name = "Name";
	Value = "Value";
	Delta = "42";
	bIsInverted = false;
	// bIsNegative = false;
}

void UOPAG_ModuleStatItem::Init
(FString _Name, FString _Value, FString _Delta, EDeltaSign _eDeltaSign, bool _bIsInverted)
{
	Name = _Name;
	Value = _Value;
	Delta = _Delta;
	bIsInverted = _bIsInverted;
	// bIsNegative = _bIsNegative;
	DeltaSign = _eDeltaSign;
}
