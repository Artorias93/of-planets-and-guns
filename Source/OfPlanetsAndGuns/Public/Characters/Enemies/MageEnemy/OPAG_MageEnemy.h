// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemies/OPAG_Enemy.h"
#include "OPAG_MageEnemy.generated.h"

/**
 * 
 */
UCLASS()
class OFPLANETSANDGUNS_API AOPAG_MageEnemy : public AOPAG_Enemy
{
	GENERATED_BODY()

public:
	AOPAG_MageEnemy();

	virtual void Attack() override;

	UFUNCTION(CallInEditor, Category = "OPAG AI")
		virtual void Death() override;

	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, Category = "Mesh")
		class UChildActorComponent* ForceField;
};
