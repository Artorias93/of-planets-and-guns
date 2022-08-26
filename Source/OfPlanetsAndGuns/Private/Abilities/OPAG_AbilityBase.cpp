// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/OPAG_AbilityBase.h"
#include "Managers/OPAG_GameInstance.h"

// Sets default values
AOPAG_AbilityBase::AOPAG_AbilityBase()
{ 	
	PrimaryActorTick.bCanEverTick = false;	
}

void AOPAG_AbilityBase::SetCaller(AActor* AbilityCaller)
{
	this->Caller = AbilityCaller;
}

// Called when the game starts or when spawned
void AOPAG_AbilityBase::BeginPlay()
{
	Super::BeginPlay();	

	EMS = GetGameInstance<UOPAG_GameInstance>()->GetEventManager();
	check(EMS);

	if (EMS != nullptr)
	{

	}
}

void AOPAG_AbilityBase::Init(EAbilityTypes AbilityType) {}