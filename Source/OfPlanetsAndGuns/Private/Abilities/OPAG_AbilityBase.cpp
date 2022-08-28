// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/OPAG_AbilityBase.h"
#include "Managers/OPAG_GameInstance.h"
#include "Components/SceneComponent.h"

// Sets default values
AOPAG_AbilityBase::AOPAG_AbilityBase()
{ 	
	PrimaryActorTick.bCanEverTick = false;
	
	Parent = CreateDefaultSubobject<USceneComponent>("Parent");
	Parent->SetupAttachment(RootComponent);

	Pivot = CreateDefaultSubobject<USceneComponent>("Pivot");
	Pivot->SetupAttachment(Parent);
}

void AOPAG_AbilityBase::SetCaller(AActor* AbilityCaller)
{
	this->Caller = AbilityCaller;
}

void AOPAG_AbilityBase::SetTarget(AActor* AbilityTarget)
{
	this->Target = AbilityTarget;
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

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AOPAG_AbilityBase::DestroyAbility, AbilityDuration, false);
}

void AOPAG_AbilityBase::DestroyAbility()
{
	Destroy();
}

void AOPAG_AbilityBase::Init(EAbilityTypes Type, float Delay)
{
	this->CastedAbilityType = Type;
	this->AbilityDelay = Delay;
}