// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemies/OPAG_Enemy.h"
#include "OPAG_PillarEnemy.generated.h"

/**
 * 
 */
UCLASS()
class OFPLANETSANDGUNS_API AOPAG_PillarEnemy : public AOPAG_Enemy
{
	GENERATED_BODY()
	
public:
	AOPAG_PillarEnemy();

	virtual void Death() override;

protected:
	UPROPERTY(EditAnywhere, Category = "Mesh")
		class UStaticMeshComponent* PillarMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Normal Turret")
		class UParticleSystem* DeathParticle;
};
