// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/OPAG_TaskBase.h"
#include "Abilities/OPAG_AbilityTypes.h"
#include "OPAG_TaskCastAbility.generated.h"


UCLASS()
class OFPLANETSANDGUNS_API UOPAG_TaskCastAbility : public UOPAG_TaskBase
{
	GENERATED_BODY()

public:
	UOPAG_TaskCastAbility(FObjectInitializer const& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
		EAbilityTypes AbilityType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
		TArray<EAbilitySpawnPattern> AvailableAbilitySpawnPattern;

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerBTComponent, uint8* NodeMemory) override;
};
