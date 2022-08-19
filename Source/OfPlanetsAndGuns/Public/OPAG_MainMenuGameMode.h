// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "OPAG_MainMenuGameMode.generated.h"

/**
 * 
 */
UCLASS()
class OFPLANETSANDGUNS_API AOPAG_MainMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()

	AOPAG_MainMenuGameMode();

	/*
	static ConstructorHelpers::FClassFinder<class AOPAG_HubHUD> HubHUDClass;

public:
	void SwitchToHubHUD();
	
	template<class T>
	void SwitchHUDClass(const T HubClass, const FString RefPath);

public:
	void SwitchToHubHUD();
	void SwitchToMainMenuHUD();
	*/
};
