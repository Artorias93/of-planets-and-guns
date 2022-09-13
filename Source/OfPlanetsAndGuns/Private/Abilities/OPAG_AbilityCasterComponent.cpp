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
#include "Kismet/GameplayStatics.h"

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

void UOPAG_AbilityCasterComponent::CastAbility(EAbilityTypes AbilityType, AActor* Caller, AActor* Target, EAbilitySpawnPattern AbilitySpawnPattern)
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	if (UClass* AbilityClass = GetAbilityClass(AbilityType))
	{
		TArray<FTransform> Transforms = GetAbilitySpawnTransforms(AbilitySpawnPattern, Caller->GetTransform());

		float IncreasingDelay = 0;
		for (auto AbilityTranfrom : Transforms)
		{
			// if the requested ability is a spawn enemy ability, apply an offset in order to fix an undesired gap (otherwise enemies spawn in the middle of the ground, falling down)
			if (AbilityType == EAbilityTypes::Spawn_DashingEnemy
				|| AbilityType == EAbilityTypes::Spawn_MachineGunEnemy
				|| AbilityType == EAbilityTypes::Spawn_MeleeEnemy
				|| AbilityType == EAbilityTypes::Spawn_PillarEnemy
				|| AbilityType == EAbilityTypes::Spawn_RangedEnemy
				|| AbilityType == EAbilityTypes::Spawn_ShieldEnemy
				|| AbilityType == EAbilityTypes::Spawn_SniperEnemy
				)
			{
				FVector AbilityTransformPosition = AbilityTranfrom.GetLocation();
				AbilityTransformPosition.Z += 30;

				AbilityTranfrom.SetLocation(AbilityTransformPosition);
			}


			FTransform SpawnTransform = AbilityTranfrom;
			AOPAG_AbilityBase* Ability = GetWorld()->SpawnActorDeferred<AOPAG_AbilityBase>(AbilityClass, SpawnTransform);

			if (bSequenced)
				IncreasingDelay += 0.5;
			
			if (Ability != nullptr)
			{
				Ability->SetCaller(Caller);
				Ability->SetTarget(Target);
				Ability->Init(AbilityType, IncreasingDelay);

				UGameplayStatics::FinishSpawningActor(Ability, AbilityTranfrom);
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

	case EAbilityTypes::Spawn_DashingEnemy:
		return SpawnEnemyAbilityClass;

	case EAbilityTypes::Spawn_MachineGunEnemy:
		return SpawnEnemyAbilityClass;

	case EAbilityTypes::Spawn_MeleeEnemy:
		return SpawnEnemyAbilityClass;

	case EAbilityTypes::Spawn_RangedEnemy:
		return SpawnEnemyAbilityClass;

	case EAbilityTypes::Spawn_SniperEnemy:
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

TArray<FTransform> UOPAG_AbilityCasterComponent::GetAbilitySpawnTransforms(EAbilitySpawnPattern AbilitySpawnPattern, FTransform Origin)
{
	TArray<FTransform> Transforms;


	/* Re-usable variables */

	FTransform Transform1 = Origin;
	FTransform Transform2 = Origin;
	FTransform Transform3 = Origin;
	FTransform Transform4 = Origin;
	FTransform Transform5 = Origin;

	FVector Position1 = Transform1.GetLocation();
	FVector Position2 = Transform1.GetLocation();
	FVector Position3 = Transform1.GetLocation();
	FVector Position4 = Transform1.GetLocation();
	FVector Position5 = Transform1.GetLocation();

	FRotator Rotation1 = Transform1.GetRotation().Rotator();
	FRotator Rotation2 = Transform1.GetRotation().Rotator();
	FRotator Rotation3 = Transform1.GetRotation().Rotator();
	FRotator Rotation4 = Transform1.GetRotation().Rotator();
	FRotator Rotation5 = Transform1.GetRotation().Rotator();
	
	switch (AbilitySpawnPattern)
	{
	case EAbilitySpawnPattern::Simple:

		Transforms.Add(Transform1);
		
		break;

	case EAbilitySpawnPattern::CenteredUpSingle:
		
		Position1.Z += 500;
		Transform1.SetLocation(Position1);
		
		Transforms.Add(Transform1);
		
		break;

	case EAbilitySpawnPattern::CenteredFrontSingle:
		
		Transforms.Add(Transform1);
		
		break;

	case EAbilitySpawnPattern::CenteredStarFour:
		
		Rotation2 += FRotator(0, 90, 0);
		Rotation3 += FRotator(0, 180, 0);
		Rotation4 += FRotator(0, 270, 0);

		Transform2.SetRotation(Rotation2.Quaternion());
		Transform3.SetRotation(Rotation3.Quaternion());
		Transform4.SetRotation(Rotation4.Quaternion());
		
		Transforms.Add(Transform1);
		Transforms.Add(Transform2);
		Transforms.Add(Transform3);
		Transforms.Add(Transform4);
		
		break;

	case EAbilitySpawnPattern::CenteredUpSequenceStar:
		
		Position1.Z += 650;
		Position2.Z += 500;
		Position3.Z += 500;
		Position4.Z += 350;
		Position5.Z += 350;

		Position2.Y += 150;
		Position3.Y -= 150;
		
		Position4.Y -= 75;
		Position5.Y += 75;

		Transform1.SetLocation(Position1);
		Transform2.SetLocation(Position2);
		Transform3.SetLocation(Position3);
		Transform4.SetLocation(Position4);
		Transform5.SetLocation(Position5);
		
		Transforms.Add(Transform1);
		Transforms.Add(Transform2);
		Transforms.Add(Transform3);
		Transforms.Add(Transform4);
		Transforms.Add(Transform5);

		bSequenced = true;
		
		break;

	case EAbilitySpawnPattern::GroundedFrontSingle:
		
		Position1.Z -= ZAxisGroundedAbilityRelativeOffset;
		Position1.X += 500;
		
		Transform1.SetLocation(Position1);

		Transforms.Add(Transform1);
		
		break;

	case EAbilitySpawnPattern::GroundedLineBackSingle:
		
		Position1.Z -= ZAxisGroundedAbilityRelativeOffset;
		Position1.X -= 500;
		
		Transform1.SetLocation(Position1);

		Transforms.Add(Transform1);
		
		break;

	case EAbilitySpawnPattern::GroundedLineBackThree:

		Position1.Z -= ZAxisGroundedAbilityRelativeOffset;
		Position2.Z -= ZAxisGroundedAbilityRelativeOffset;
		Position3.Z -= ZAxisGroundedAbilityRelativeOffset;
		
		Position1.X -= 500;
		Position2 = Position1;
		Position2.Y += 650;
		Position3 = Position1;
		Position3.Y -= 650;
		
		Transform1.SetLocation(Position1);
		Transform2.SetLocation(Position2);
		Transform3.SetLocation(Position3);

		Transforms.Add(Transform1);
		Transforms.Add(Transform2);
		Transforms.Add(Transform3);
		
		break;

	case EAbilitySpawnPattern::GroundedLineBackTwo:

		Position1.Z -= ZAxisGroundedAbilityRelativeOffset;
		Position2.Z -= ZAxisGroundedAbilityRelativeOffset;
		Position3.Z -= ZAxisGroundedAbilityRelativeOffset;
		
		Position1.X -= 500;
		Position2 = Position1;
		Position2.Y += 650;
		Position3 = Position1;
		Position3.Y -= 650;
		
		Transform2.SetLocation(Position2);
		Transform3.SetLocation(Position3);

		Transforms.Add(Transform2);
		Transforms.Add(Transform3);
		
		break;

	case EAbilitySpawnPattern::GroundedLineFrontTwo:

		Position1.Z -= ZAxisGroundedAbilityRelativeOffset;
		Position2.Z -= ZAxisGroundedAbilityRelativeOffset;
		Position3.Z -= ZAxisGroundedAbilityRelativeOffset;
		
		Position1.X += 500;
		Position2 = Position1;
		Position2.Y += 650;
		Position3 = Position1;
		Position3.Y -= 650;
		
		Transform2.SetLocation(Position2);
		Transform3.SetLocation(Position3);

		Transforms.Add(Transform2);
		Transforms.Add(Transform3);
		
		break;

	case EAbilitySpawnPattern::GroundedTridentBackThree:

		Position1.Z -= ZAxisGroundedAbilityRelativeOffset;
		Position2.Z -= ZAxisGroundedAbilityRelativeOffset;
		Position3.Z -= ZAxisGroundedAbilityRelativeOffset;
		
		Position1.X -= 500;
		Position2 = Position1;
		Position2.X -= 500;
		Position2.Y += 650;
		Position3 = Position1;
		Position3.X -= 500;
		Position3.Y -= 650;
		
		Position1.X -= 800;

		Transform1.SetLocation(Position1);
		Transform2.SetLocation(Position2);
		Transform3.SetLocation(Position3);

		Transforms.Add(Transform1);
		Transforms.Add(Transform2);
		Transforms.Add(Transform3);
		
		break;

	case EAbilitySpawnPattern::GroundedTridentFrontThree:
		
		Position1.Z -= ZAxisGroundedAbilityRelativeOffset;
		Position2.Z -= ZAxisGroundedAbilityRelativeOffset;
		Position3.Z -= ZAxisGroundedAbilityRelativeOffset;
		
		Position1.X += 500;
		Position2 = Position1;
		Position2.X -= 500;
		Position2.Y += 650;
		Position3 = Position1;
		Position3.X -= 500;
		Position3.Y -= 650;

		Position1.X += 800;

		Transform1.SetLocation(Position1);
		Transform2.SetLocation(Position2);
		Transform3.SetLocation(Position3);

		Transforms.Add(Transform1);
		Transforms.Add(Transform2);
		Transforms.Add(Transform3);
		
		break;

	case EAbilitySpawnPattern::SingleBackLeftCorner:

		Position1.X -= 750;
		Position1.Y -= 750;

		Transform1.SetLocation(Position1);

		Transforms.Add(Transform1);
		
		break;

	case EAbilitySpawnPattern::SingleBackRightCorner:

		Position1.X -= 750;
		Position1.Y += 750;

		Transform1.SetLocation(Position1);

		Transforms.Add(Transform1);
		
		break;

	case EAbilitySpawnPattern::SingleFrontLeftCorner:

		Position1.X += 750;
		Position1.Y -= 750;

		Transform1.SetLocation(Position1);

		Transforms.Add(Transform1);
		
		break;

	case EAbilitySpawnPattern::SingleFrontRightCorner:

		Position1.X += 750;
		Position1.Y += 750;

		Transform1.SetLocation(Position1);

		Transforms.Add(Transform1);
		
		break;
	
	case EAbilitySpawnPattern::GroundedLineFrontThree:

		Position1.Z -= ZAxisGroundedAbilityRelativeOffset;
		Position2.Z -= ZAxisGroundedAbilityRelativeOffset;
		Position3.Z -= ZAxisGroundedAbilityRelativeOffset;
		
		Position1.X += 500;
		Position2 = Position1;
		Position2.Y += 650;
		Position3 = Position1;
		Position3.Y -= 650;
		
		Transform1.SetLocation(Position1);
		Transform2.SetLocation(Position2);
		Transform3.SetLocation(Position3);

		Transforms.Add(Transform1);
		Transforms.Add(Transform2);
		Transforms.Add(Transform3);
		
		break;
		
	default:
		return Transforms;
	}

	return Transforms;
}

