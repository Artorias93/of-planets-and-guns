// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Managers/OPAG_EventManagerSubsystem.h"
#include "Abilities/OPAG_AbilityTypes.h"
#include "OPAG_AbilityBase.generated.h"

UCLASS()
class OFPLANETSANDGUNS_API AOPAG_AbilityBase : public AActor
{
	GENERATED_BODY()		

public:	
	AOPAG_AbilityBase();

public:
	void SetCaller(AActor* Caller);
	void SetTarget(AActor* Target);
	virtual void Init(EAbilityTypes AbilityType, float Delay = 0);

protected:
	virtual void BeginPlay() override;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
		class USceneComponent* Pivot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	class USceneComponent* Parent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
		float AbilityDuration;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
		float AbilityDamage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
		float AbilityDelay = 0;

	UPROPERTY()
		EAbilityTypes CastedAbilityType;
	
	UPROPERTY()
		UOPAG_EventManagerSubsystem* EMS;

	UPROPERTY()
		AActor* Caller;	

	UPROPERTY()
		AActor* Target;

	virtual void DestroyAbility();
};
