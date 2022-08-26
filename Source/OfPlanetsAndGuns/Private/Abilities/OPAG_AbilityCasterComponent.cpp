// Fill out your copyright notice in the Description page of Project Settings.

#include "Abilities/OPAG_AbilityCasterComponent.h"
#include "Managers/OPAG_EventManagerSubsystem.h"
#include "Managers/OPAG_GameInstance.h"
#include "Abilities/OPAG_AbilityBase.h"
#include "Abilities/OPAG_ForceField.h"
#include "Abilities/OPAG_SpawnEnemy.h"
#include "UObject/ConstructorHelpers.h"
#include "Abilities/OPAG_AbilityTypes.h"

UOPAG_AbilityCasterComponent::UOPAG_AbilityCasterComponent()
{
	PrimaryComponentTick.bStartWithTickEnabled = false;

	/*static ConstructorHelpers::FClassFinder<AOPAG_ForceField> ForceFieldClass(TEXT("Class'/Game/OfPlanetsAndGuns/Characters/MageEnemy/BP_ForceField.BP_ForceField_C'"));
	if (ForceFieldClass.Succeeded())
	{
		ForceFieldAbilityClass = ForceFieldClass.Class;
	}

	static ConstructorHelpers::FClassFinder<AOPAG_SpawnEnemy> SpawnEnemyClass(TEXT("Class'/Game/OfPlanetsAndGuns/Characters/MageEnemy/BP_SpawnEnemy.BP_SpawnEnemy_C'"));
	if (SpawnEnemyClass.Succeeded())
	{
		SpawnEnemyAbilityClass = SpawnEnemyClass.Class;
	}*/
}

void UOPAG_AbilityCasterComponent::BeginPlay()
{
	Super::BeginPlay();

	FString ForceFieldClassPath = "Class'/Game/OfPlanetsAndGuns/Characters/MageEnemy/BP_ForceField.BP_ForceField_C'";
	FString SpawnEnemyClassPath = "Class'/Game/OfPlanetsAndGuns/Characters/MageEnemy/BP_SpawnEnemy.BP_SpawnEnemy_C'";

	const TCHAR* ForceFieldClass= *ForceFieldClassPath;
	const TCHAR* SpawnEnemyClass = *SpawnEnemyClassPath;

	ForceFieldAbilityClass = LoadObject<UClass>(nullptr, ForceFieldClass);
	SpawnEnemyAbilityClass = LoadObject<UClass>(nullptr, SpawnEnemyClass);

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

