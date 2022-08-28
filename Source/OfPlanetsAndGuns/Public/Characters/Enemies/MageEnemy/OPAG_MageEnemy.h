// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemies/OPAG_Enemy.h"
#include "OPAG_MageEnemy.generated.h"

UENUM(BlueprintType)
enum FIGHT_STATUS
{
	FIRST UMETA(DisplayName = "FIRST PHASE"),
	SECOND UMETA(DisplayName = "SECOND PHASE")
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

	UPROPERTY(EditAnywhere, Category = "OPAG AI")
		class AOPAG_ForceField* ForceField;

	UPROPERTY()
	class AOPAG_AbilityCaster* AbilityCaster;

private:
	FIGHT_STATUS FightStatus = FIGHT_STATUS::FIRST;

	int AvailablePillarsAmount = 0;

	int DestroyedPillarCounter = 0;

	TArray<class AActor*> Pillars;		
};
