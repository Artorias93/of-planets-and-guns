// Fill out your copyright notice in the Description page of Project Settings.


#include "OPAG_HubGameMode.h"
#include "UI/OPAG_HubHUD.h"

AOPAG_HubGameMode::AOPAG_HubGameMode()
{
	static ConstructorHelpers::FClassFinder<AOPAG_HubHUD> HubHUDClass(TEXT("/Game/OfPlanetsAndGuns/UI/BP_HubHUD"));

	if (HubHUDClass.Class != NULL) {
		HUDClass = HubHUDClass.Class;
	}
	
}
