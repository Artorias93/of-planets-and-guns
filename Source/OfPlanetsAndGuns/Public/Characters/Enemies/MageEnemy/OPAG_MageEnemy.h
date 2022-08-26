// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemies/OPAG_Enemy.h"
#include "OPAG_MageEnemy.generated.h"

UENUM()
enum EFightStatus
{
	FirstPhase UMETA(DisplayName = "FIRST PHASE"),
	SecondPhase UMETA(DisplayName = "SECOND PHASE")
};

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

	UFUNCTION(CallInEditor, Category = "OPAG AI")
		void OnEnemyDeath(const AOPAG_Enemy* const Enemy);

	UFUNCTION(CallInEditor, Category = "OPAG AI")
		void OnBrokenForceField();	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Caster")
		class UOPAG_AbilityCasterComponent* AbilityCaster;

private:
	EFightStatus FightStatus = EFightStatus::FirstPhase;
};
