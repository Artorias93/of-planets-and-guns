// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemies/OPAG_Enemy.h"
#include "Abilities/OPAG_AbilityTypes.h"
#include "OPAG_MageEnemy.generated.h"

UENUM()
enum EFightStatus
{
	FirstPhase UMETA(DisplayName = "FIRST PHASE"),
	SecondPhase UMETA(DisplayName = "SECOND PHASE")
};

UENUM()
enum EWaveLevel
{
	WaveLevel1,
	WaveLevel2
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
	virtual void Tick(float DeltaTime) override;


	void CastAbilityRequest(EAbilityTypes AbilityType, AActor* Caller, AActor* Target, EAbilitySpawnPattern AbilitySpawnPattern);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* CastAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CastAnimMontagePlayRate = 1.f;
	
protected:

	UFUNCTION(CallInEditor, Category = "OPAG AI")
		void OnEnemyDeath(const AOPAG_Enemy* const Enemy);

	UFUNCTION(CallInEditor, Category = "OPAG AI")
		void OnSpawnedEnemy(AOPAG_Enemy* Enemy, AActor* Caller);

	UFUNCTION(CallInEditor, Category = "OPAG AI")
		void OnBrokenForceField();

	FVector GetRandomPointInRadius();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Caster")
		class UOPAG_AbilityCasterComponent* AbilityCaster;

	UPROPERTY()
		TArray<TSoftObjectPtr<AOPAG_Enemy>> CastedEnemiesList;

private:
	
	void Init();
	float GetGroundHeight();
	
	EFightStatus FightStatus = EFightStatus::FirstPhase;

	// vars to track if the enemy life is at or lower a specific value
	bool bHealth25 = false;
	bool bHealth50 = false;
};
