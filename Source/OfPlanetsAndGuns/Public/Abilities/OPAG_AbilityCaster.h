// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OPAG_AbilityCaster.generated.h"

class UOPAG_EventManagerSubsystem;

UCLASS()
class OFPLANETSANDGUNS_API AOPAG_AbilityCaster : public AActor
{
	GENERATED_BODY()
	
public:	
	AOPAG_AbilityCaster();	

protected:
	UPROPERTY()
		class UOPAG_EventManagerSubsystem* EMS;

	virtual void BeginPlay() override;

	UFUNCTION(CallInEditor, Category = "Ability Caster")
		void CastAbility(TSubclassOf<class AOPAG_AbilityBase> AbilityClass, const AActor* Caller, FVector Position, AOPAG_AbilityBase* CastedAbility);
};
