// Fill out your copyright notice in the Description page of Project Settings.


#include "OPAG_MainMenuGameMode.h"
#include "UI/OPAG_MainMenuHUD.h"

AOPAG_MainMenuGameMode::AOPAG_MainMenuGameMode()
{
	static ConstructorHelpers::FClassFinder<AOPAG_MainMenuHUD> MainMenuHUDClass(TEXT("/Game/OfPlanetsAndGuns/UI/BP_MainMenuHUD"));

	if (MainMenuHUDClass.Class != NULL) {
		HUDClass = MainMenuHUDClass.Class;
	}
	
}
