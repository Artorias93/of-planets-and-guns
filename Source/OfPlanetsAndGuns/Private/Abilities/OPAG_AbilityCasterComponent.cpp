// Fill out your copyright notice in the Description page of Project Settings.

#include "Abilities/OPAG_AbilityCasterComponent.h"
#include "Managers/OPAG_EventManagerSubsystem.h"
#include "Managers/OPAG_GameInstance.h"
#include "Abilities/OPAG_AbilityBase.h"
#include "Abilities/OPAG_ForceField.h"
#include "Abilities/OPAG_SpawnEnemy.h"
#include "Abilities/OPAG_TornadoAbility.h"
#include "UObject/ConstructorHelpers.h"
#include "Abilities/OPAG_AbilityTypes.h"

UOPAG_AbilityCasterComponent::UOPAG_AbilityCasterComponent()
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UOPAG_AbilityCasterComponent::BeginPlay()
{
	Super::BeginPlay();

	FString ForceFieldClassPath = "Class'/Game/OfPlanetsAndGuns/Characters/MageEnemy/BP_ForceField.BP_ForceField_C'";
	FString SpawnEnemyClassPath = "Class'/Game/OfPlanetsAndGuns/Characters/MageEnemy/BP_SpawnEnemy.BP_SpawnEnemy_C'";
	FString TornadoClassPath = "Class'/Game/OfPlanetsAndGuns/Characters/MageEnemy/BP_TornadoAbility.BP_TornadoAbility_C'";

	const TCHAR* ForceFieldClass= *ForceFieldClassPath;
	const TCHAR* SpawnEnemyClass = *SpawnEnemyClassPath;
	const TCHAR* TornadoClass = *TornadoClassPath;

	ForceFieldAbilityClass = LoadObject<UClass>(nullptr, ForceFieldClass);
	SpawnEnemyAbilityClass = LoadObject<UClass>(nullptr, SpawnEnemyClass);
	TornadoAbilityClass = LoadObject<UClass>(nullptr, TornadoClass);

	EMS = GetWorld()->GetGameInstance<UOPAG_GameInstance>()->GetEventManager();
	check(EMS);

	if (EMS != nullptr)
	{
		EMS->CastAbility.AddDynamic(this,
			&UOPAG_AbilityCasterComponent::CastAbility);
	}
}

void UOPAG_AbilityCasterComponent::CastAbility(EAbilityTypes AbilityType, AActor* Caller, FVector Position)
{
	UClass* AbilityClass = nullptr;

	switch (AbilityType)
	{
	case EAbilityTypes::ForceField:
		AbilityClass = ForceFieldAbilityClass;
		break;

	case EAbilityTypes::Spawn_PillarEnemy:
		AbilityClass = SpawnEnemyAbilityClass;
		break;

	case EAbilityTypes::Spawn_ShieldEnemy:
		AbilityClass = SpawnEnemyAbilityClass;
		break;

	case EAbilityTypes::Tornado:
		AbilityClass = TornadoAbilityClass;
		break;

	default:
		return;
	}

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	if (AbilityClass != nullptr)
	{
		AOPAG_AbilityBase* Ability = GetWorld()->SpawnActor<AOPAG_AbilityBase>(AbilityClass, Position, FRotator::ZeroRotator, SpawnInfo);

		Ability->SetCaller(Caller);
		Ability->Init(AbilityType);
	}	
}

