// Fill out your copyright notice in the Description page of Project Settings.


#include "OPAG_MainMenuGameMode.h"
#include "UI/OPAG_MainMenuHUD.h"
#include "UI/OPAG_HubHUD.h"

AOPAG_MainMenuGameMode::AOPAG_MainMenuGameMode()
{
	static ConstructorHelpers::FClassFinder<AOPAG_MainMenuHUD> MainMenuHUDClass(TEXT("/Game/OfPlanetsAndGuns/UI/BP_MainMenuHUD"));

	if (MainMenuHUDClass.Class != nullptr) {
		HUDClass = MainMenuHUDClass.Class;
	}
}

/*
void AOPAG_MainMenuGameMode::SwitchToHubHUD()
{
	//static ConstructorHelpers::FClassFinder<AOPAG_HubHUD> HubHUDClass(TEXT("/Game/OfPlanetsAndGuns/UI/BP_HubHUD"));

	if (HubHUDClass != NULL) {
		HUDClass = HubHUDClass;
	}
}

//void AOPAG_MainMenuGameMode::SwitchToHubHUD()
//{
//	SwitchHUDClass(AOPAG_HubHUD::StaticClass(), "/Game/OfPlanetsAndGuns/UI/BP_HubHUD");
//}
//
//void AOPAG_MainMenuGameMode::SwitchToMainMenuHUD()
//{
//	SwitchHUDClass(AOPAG_MainMenuHUD::StaticClass(), "/Game/OfPlanetsAndGuns/UI/BP_MainMenuHUD");
//}
//
//template<class T>
//void AOPAG_MainMenuGameMode::SwitchHUDClass(const T HUDClassEntry, const FString RefPath)
//{
//	static ConstructorHelpers::FClassFinder<HUDClassEntry> NewHUDClass(TEXT(RefPath));
//
//	if (NewHUDClass.Class) {
//		HUDClass = NewHUDClass.Class;
//	}
//}

*/