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
	UFUNCTION(CallInEditor, Category = "OPAG AI")
		virtual void Death() override;

protected:
	UPROPERTY(EditAnywhere, Category = "Mesh")
		class UStaticMeshComponent* PillarMesh;
};
