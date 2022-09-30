// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OPAG_AbilityTypes.h"
#include "Components/ActorComponent.h"
#include "OPAG_AbilityCasterComponent.generated.h"

UCLASS(BlueprintType)
class OFPLANETSANDGUNS_API UOPAG_AbilityCasterComponent : public UActorComponent
{
	GENERATED_BODY()

	bool bSequenced = false;
	
public:

	UOPAG_AbilityCasterComponent();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
		float ZAxisGroundedAbilityRelativeOffset = 270;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
		TSubclassOf<class AOPAG_ForceField> ForceFieldAbilityClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
		TSubclassOf<class AOPAG_SpawnEnemy> SpawnEnemyAbilityClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
		TSubclassOf<class AOPAG_TornadoAbility> TornadoAbilityClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
		TSubclassOf<class AOPAG_CrackAbility> CrackAbilityClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
		TSubclassOf<class AOPAG_RayAbility> RayAbilityClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
		TSubclassOf<class AOPAG_SupernovaAbility> SupernovaAbilityClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
		TSubclassOf<class AOPAG_FireballAbility> FireballAbilityClass;
	
	UPROPERTY()
		class UOPAG_EventManagerSubsystem* EMS;

	virtual void BeginPlay() override;

	UFUNCTION(CallInEditor, Category = "Ability Caster")
		void CastAbility(enum EAbilityTypes AbilityEnum, AActor* Caller, AActor* Target, enum EAbilitySpawnPattern AbilitySpawnPattern);

	UFUNCTION(CallInEditor, Category = "Ability Caster")
		UClass* GetAbilityClass(EAbilityTypes AbilityType) const;
	
	UFUNCTION(CallInEditor, Category = "Ability Caster")
		TArray<FTransform> GetAbilitySpawnTransforms(EAbilitySpawnPattern AbilitySpawnPattern, FTransform Origin);

private:
	void LoadAbilitiesAssets();
};
