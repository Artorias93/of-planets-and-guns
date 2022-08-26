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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	float Duration;

public:
	void SetCaller(AActor* Caller);
	void SetTarget(const AActor* Target);
	virtual void Init(EAbilityTypes AbilityType);

protected:
	virtual void BeginPlay() override;	

	UPROPERTY()
		UOPAG_EventManagerSubsystem* EMS;

	UPROPERTY()
		AActor* Caller;	

	UPROPERTY()
		const AActor* Target;

	virtual void DestroyAbility();
};
