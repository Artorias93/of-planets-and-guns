// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/OPAG_AbilityBase.h"
#include "OPAG_TornadoAbility.generated.h"

/**
 * 
 */
UCLASS()
class OFPLANETSANDGUNS_API AOPAG_TornadoAbility : public AOPAG_AbilityBase
{
	GENERATED_BODY()

public:
	AOPAG_TornadoAbility();
	virtual void Init(EAbilityTypes AbilityType) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Mesh")
		class UStaticMeshComponent* AbilityMesh;

	UPROPERTY(EditAnywhere, Category = "Collider")
		class UBoxComponent* AbilityCollider;

	UPROPERTY(EditAnywhere, Category = "Particle")
		class UParticleSystemComponent* AbilityParticle;
};
