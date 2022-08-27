// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/OPAG_AbilityBase.h"
#include "OPAG_ForceField.generated.h"

UCLASS()
class OFPLANETSANDGUNS_API AOPAG_ForceField : public AOPAG_AbilityBase
{
	GENERATED_BODY()
	
public:	
	AOPAG_ForceField();	
	virtual void Init(EAbilityTypes AbilityType, float Delay) override;

protected:
	virtual void BeginPlay() override;		

	UPROPERTY(EditAnywhere, Category = "Mesh")
		class UStaticMeshComponent* AbilityMesh;

	UFUNCTION()
	void OnPillarDestroyed();

	UFUNCTION()
	virtual void DestroyAbility() override;

	int AvailablePillarsAmount = 0;

	TArray<AActor*> Pillars;
};
