// Fill out your copyright notice in the Description page of Project Settings.


#include "OPAG_BaseGameMode.h"
#include "UI/OPAG_GameplayHUD.h"

AOPAG_BaseGameMode::AOPAG_BaseGameMode() {

	static ConstructorHelpers::FClassFinder<AOPAG_GameplayHUD> GameplayHUDClass(TEXT("/Game/OfPlanetsAndGuns/UI/BP_GameplayHUD"));

	if (GameplayHUDClass.Class != NULL) {
		HUDClass = GameplayHUDClass.Class;
	}
}