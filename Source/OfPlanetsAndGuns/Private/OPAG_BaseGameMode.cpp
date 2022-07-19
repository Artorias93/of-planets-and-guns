// Fill out your copyright notice in the Description page of Project Settings.


#include "OPAG_BaseGameMode.h"

#include "Characters/Player/OPAG_Player.h"
#include "Characters/Player/OPAG_PlayerController.h"
#include "UI/OPAG_GameplayHUD.h"

AOPAG_BaseGameMode::AOPAG_BaseGameMode() {

	static ConstructorHelpers::FClassFinder<AOPAG_GameplayHUD> GameplayHUDClass(TEXT("/Game/OfPlanetsAndGuns/UI/BP_GameplayHUD"));

	if (GameplayHUDClass.Class != nullptr) {
		HUDClass = GameplayHUDClass.Class;
	}

	static ConstructorHelpers::FClassFinder<AOPAG_Player> PlayerClass(TEXT("/Game/OfPlanetsAndGuns/Characters/Player/BP_Player"));

	if(PlayerClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerClass.Class;
	}

	PlayerControllerClass = AOPAG_PlayerController::StaticClass();
}