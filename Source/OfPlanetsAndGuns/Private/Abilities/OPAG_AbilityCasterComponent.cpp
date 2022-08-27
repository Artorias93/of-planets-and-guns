// Fill out your copyright notice in the Description page of Project Settings.

#include "Abilities/OPAG_AbilityCasterComponent.h"
#include "Managers/OPAG_EventManagerSubsystem.h"
#include "Managers/OPAG_GameInstance.h"
#include "Abilities/OPAG_AbilityTypes.h"
#include "Abilities/OPAG_AbilityBase.h"
#include "Abilities/OPAG_ForceField.h"
#include "Abilities/OPAG_SpawnEnemy.h"
#include "Abilities/OPAG_TornadoAbility.h"
#include "Abilities/OPAG_CrackAbility.h"
#include "Abilities/OPAG_RayAbility.h"
#include "Abilities/OPAG_FireballAbility.h"
#include "Abilities/OPAG_SupernovaAbility.h"

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
	FString CrackClassPath = "Class'/Game/OfPlanetsAndGuns/Characters/MageEnemy/BP_CrackAbility.BP_CrackAbility_C'";
	FString RayClassPath = "Class'/Game/OfPlanetsAndGuns/Characters/MageEnemy/BP_RayAbility.BP_RayAbility_C'";
	FString SupernovaClassPath = "Class'/Game/OfPlanetsAndGuns/Characters/MageEnemy/BP_SupernovaAbility.BP_SupernovaAbility_C'";
	FString FireballClassPath = "Class'/Game/OfPlanetsAndGuns/Characters/MageEnemy/BP_FireballAbility.BP_FireballAbility_C'";

	const TCHAR* ForceFieldClass= *ForceFieldClassPath;
	const TCHAR* SpawnEnemyClass = *SpawnEnemyClassPath;
	const TCHAR* TornadoClass = *TornadoClassPath;
	const TCHAR* CrackClass = *CrackClassPath;
	const TCHAR* RayClass = *RayClassPath;
	const TCHAR* SupernovaClass = *SupernovaClassPath;
	const TCHAR* FireballClass = *FireballClassPath;

	ForceFieldAbilityClass = LoadObject<UClass>(nullptr, ForceFieldClass);
	SpawnEnemyAbilityClass = LoadObject<UClass>(nullptr, SpawnEnemyClass);
	TornadoAbilityClass = LoadObject<UClass>(nullptr, TornadoClass);
	CrackAbilityClass = LoadObject<UClass>(nullptr, CrackClass);
	RayAbilityClass = LoadObject<UClass>(nullptr, RayClass);
	SupernovaAbilityClass = LoadObject<UClass>(nullptr, SupernovaClass);
	FireballAbilityClass = LoadObject<UClass>(nullptr, FireballClass);

	EMS = GetWorld()->GetGameInstance<UOPAG_GameInstance>()->GetEventManager();
	check(EMS);

	if (EMS != nullptr)
	{
		EMS->CastAbility.AddDynamic(this,
			&UOPAG_AbilityCasterComponent::CastAbility);
	}
}

void UOPAG_AbilityCasterComponent::CastAbility(EAbilityTypes AbilityType, AActor* Caller, const AActor* Target, EAbilitySpawnPattern AbilitySpawnPattern)
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	if (UClass* AbilityClass = GetAbilityClass(AbilityType))
	{
		TArray<FVector> Positions = GetAbilitySpawnPositions(AbilitySpawnPattern, Caller->GetActorLocation());

		float IncreasingDelay = 0;
		for (auto AbilityPosition : Positions)
		{
			AOPAG_AbilityBase* Ability = GetWorld()->SpawnActor<AOPAG_AbilityBase>(AbilityClass, AbilityPosition, FRotator::ZeroRotator, SpawnInfo);

			if (bSequenced)
				IncreasingDelay += 1.5;
			
			if (Ability != nullptr)
			{
				Ability->SetCaller(Caller);
				Ability->SetTarget(Target);
				Ability->Init(AbilityType, IncreasingDelay);
			}
		}

		bSequenced = false;
	}	
}

UClass* UOPAG_AbilityCasterComponent::GetAbilityClass(EAbilityTypes AbilityType) const
{
	switch (AbilityType)
	{
	case EAbilityTypes::ForceField:
		return ForceFieldAbilityClass;

	case EAbilityTypes::Spawn_PillarEnemy:
		return  SpawnEnemyAbilityClass;

	case EAbilityTypes::Spawn_ShieldEnemy:
		return SpawnEnemyAbilityClass;

	case EAbilityTypes::Tornado:
		return TornadoAbilityClass;
		
	case EAbilityTypes::Crack:
		return CrackAbilityClass;
		
	case EAbilityTypes::Ray:
		return RayAbilityClass;

	case EAbilityTypes::Supernova:
		return SupernovaAbilityClass;

	case EAbilityTypes::Fireball:
		return FireballAbilityClass;
		
	default:
		return nullptr;
	}
}

TArray<FVector> UOPAG_AbilityCasterComponent::GetAbilitySpawnPositions(EAbilitySpawnPattern AbilitySpawnPattern, FVector Origin)
{
	TArray<FVector> Positions;

	FVector Position1 = Origin;
	FVector Position2 = Origin;
	FVector Position3 = Origin;
	FVector Position4 = Origin;
	FVector Position5 = Origin;
	
	switch (AbilitySpawnPattern)
	{
	case EAbilitySpawnPattern::Simple:

		Positions.Add(Position1);
		
		break;

	case EAbilitySpawnPattern::CenteredUpSingle:
		
		Position1.Z += 250;
		
		Positions.Add(Position1);
		
		break;

	case EAbilitySpawnPattern::CenteredFrontSingle:
		
		Positions.Add(Position1);
		
		break;

	case EAbilitySpawnPattern::CenteredStarFour:
		
		Position2 = Position2.RotateAngleAxis(90, FVector(0, 0, 1));
		Position3 = Position3.RotateAngleAxis(180, FVector(0, 0, 1));
		Position4 = Position4.RotateAngleAxis(270, FVector(0, 0, 1));
		
		Positions.Add(Position1);
		Positions.Add(Position2);
		Positions.Add(Position3);
		Positions.Add(Position4);
		
		break;

	case EAbilitySpawnPattern::CenteredUpSequenceStar:
		
		Position1.Z += 400;
		Position2.Z += 250;
		Position3.Z += 250;
		Position4.Z += 100;
		Position5.Z += 100;

		Position2.Y += 150;
		Position3.Y -= 150;
		
		Position4.Y -= 75;
		Position5.Y += 75;
		
		Positions.Add(Position1);
		Positions.Add(Position2);
		Positions.Add(Position3);
		Positions.Add(Position4);
		Positions.Add(Position5);

		bSequenced = true;
		
		break;

	case EAbilitySpawnPattern::GroundedFrontSingle:
		
		Position1.Z = 0;
		Position1.X += 100;
		
		Positions.Add(Position1);
		
		break;

	case EAbilitySpawnPattern::GroundedLineBackSingle:
		
		Position1.Z = 0;
		Position1.X -= 300;
		
		Positions.Add(Position1);
		
		break;

	case EAbilitySpawnPattern::GroundedLineBackThree:

		Position1.Z = 0;
		Position2.Z = 0;
		Position3.Z = 0;
		
		Position1.X -= 300;
		Position2 = Position1;
		Position2.Y += 500;
		Position3 = Position1;
		Position3.Y -= 500;
		
		Positions.Add(Position1);
		Positions.Add(Position2);
		Positions.Add(Position3);
		
		break;

	case EAbilitySpawnPattern::GroundedLineBackTwo:

		Position1.Z = 0;
		Position2.Z = 0;
		Position3.Z = 0;
		
		Position1.X -= 300;
		Position2 = Position1;
		Position2.Y += 300;
		Position3 = Position1;
		Position3.Y -= 300;
		
		Positions.Add(Position2);
		Positions.Add(Position3);
		
		break;

	case EAbilitySpawnPattern::GroundedLineFrontTwo:

		Position1.Z = 0;
		Position2.Z = 0;
		Position3.Z = 0;
		
		Position1.X += 100;
		Position2 = Position1;
		Position2.Y += 300;
		Position3 = Position1;
		Position3.Y -= 300;
		
		Positions.Add(Position2);
		Positions.Add(Position3);
		
		break;

	case EAbilitySpawnPattern::GroundedTridentBackThree:

		Position1.Z = 0;
		Position2.Z = 0;
		Position3.Z = 0;
		
		Position1.X -= 300;
		Position2 = Position1;
		Position2.X -= 300;
		Position2.Y += 300;
		Position3 = Position1;
		Position3.X -= 300;
		Position3.Y -= 300;
		
		Position1.X += 100;
		Positions.Add(Position2);
		Positions.Add(Position3);
		
		break;

	case EAbilitySpawnPattern::GroundedTridentFrontThree:
		
		Position1.Z = 0;
		Position2.Z = 0;
		Position3.Z = 0;
		
		Position1.X += 300;
		Position2 = Position1;
		Position2.X -= 300;
		Position2.Y += 300;
		Position3 = Position1;
		Position3.X -= 300;
		Position3.Y -= 300;

		Positions.Add(Position1);
		Positions.Add(Position2);
		Positions.Add(Position3);
		
		break;

	case EAbilitySpawnPattern::SingleBackLeftCorner:

		Position1.X -= 750;
		Position1.Y -= 750;

		Positions.Add(Position1);
		
		break;

	case EAbilitySpawnPattern::SingleBackRightCorner:

		Position1.X -= 750;
		Position1.Y += 750;

		Positions.Add(Position1);
		
		break;

	case EAbilitySpawnPattern::SingleFrontLeftCorner:

		Position1.X += 750;
		Position1.Y -= 750;

		Positions.Add(Position1);
		
		break;

	case EAbilitySpawnPattern::SingleFrontRightCorner:

		Position1.X += 750;
		Position1.Y += 750;

		Positions.Add(Position1);
		
		break;
	
	case EAbilitySpawnPattern::GroundedLineFrontThree:

		Position1.Z = 0;
		Position2.Z = 0;
		Position3.Z = 0;
		
		Position1.X += 100;
		Position2 = Position1;
		Position2.Y += 500;
		Position3 = Position1;
		Position3.Y -= 500;
		
		Positions.Add(Position1);
		Positions.Add(Position2);
		Positions.Add(Position3);
		
		break;
		
	default:
		return Positions;
	}

	return Positions;
}

