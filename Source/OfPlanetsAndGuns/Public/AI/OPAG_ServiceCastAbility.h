// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/OPAG_ServiceBase.h"
#include "Abilities/OPAG_AbilityTypes.h"
#include "OPAG_ServiceCastAbility.generated.h"

/**
 * 
 */
UCLASS()
class OFPLANETSANDGUNS_API UOPAG_ServiceCastAbility : public UOPAG_ServiceBase
{
	GENERATED_BODY()
	
public:
	UOPAG_ServiceCastAbility(FObjectInitializer const& ObjectInitializer);

	virtual void TickNode(UBehaviorTreeComponent& OwnerBTComponent, uint8* NodeMemory, float DeltaSeconds) override;

	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerBTComponent, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
		EAbilityTypes AbilityType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
		EAbilitySpawnPattern AbilitySpawnPattern;
};
