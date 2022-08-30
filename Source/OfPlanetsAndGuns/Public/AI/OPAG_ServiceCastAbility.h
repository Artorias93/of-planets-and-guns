// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/OPAG_ServiceBase.h"
#include "Abilities/OPAG_AbilityTypes.h"
#include "OPAG_ServiceCastAbility.generated.h"

USTRUCT(BlueprintType)
struct OFPLANETSANDGUNS_API FOPAG_ServiceAbilityElement : public FTableRowBase
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EAbilityTypes AbilityType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<EAbilitySpawnPattern> AvailableAbilitySpawnPattern;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Weight;
};

UCLASS()
class OFPLANETSANDGUNS_API UOPAG_ServiceCastAbility : public UOPAG_ServiceBase
{
	GENERATED_BODY()
	
public:
	UOPAG_ServiceCastAbility(FObjectInitializer const& ObjectInitializer);

	virtual void TickNode(UBehaviorTreeComponent& OwnerBTComponent, uint8* NodeMemory, float DeltaSeconds) override;

	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerBTComponent, uint8* NodeMemory) override;

	FOPAG_ServiceAbilityElement GetRandomAbilityDefinitionElement();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
		TArray<FOPAG_ServiceAbilityElement> AbilitiesDefinitions;
};
