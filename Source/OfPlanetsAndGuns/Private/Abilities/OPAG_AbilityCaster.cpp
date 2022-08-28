// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/OPAG_AbilityCaster.h"
#include "Managers/OPAG_EventManagerSubsystem.h"
#include "Managers/OPAG_GameInstance.h"
#include "Abilities/OPAG_AbilityBase.h"

AOPAG_AbilityCaster::AOPAG_AbilityCaster()
{ 	
	PrimaryActorTick.bCanEverTick = false;
}

void AOPAG_AbilityCaster::BeginPlay()
{
	Super::BeginPlay();	

	EMS = GetGameInstance<UOPAG_GameInstance>()->GetEventManager();
	check(EMS);

	if (EMS != nullptr)
	{
		EMS->CastAbility.AddDynamic(this,
			&AOPAG_AbilityCaster::CastAbility);
	}
}

void AOPAG_AbilityCaster::CastAbility(TSubclassOf<AOPAG_AbilityBase> AbilityClass, const AActor* Caller, FVector Position, AOPAG_AbilityBase* CastedAbility)
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AOPAG_AbilityBase* Ability = GetWorld()->SpawnActor<AOPAG_AbilityBase>(AbilityClass, Position, Position.ToOrientationRotator(), SpawnInfo);

	if (Ability != nullptr)
	{
		CastedAbility = Ability;
	}
}




