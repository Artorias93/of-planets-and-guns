// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemies/OPAG_Enemy.h"
#include "OPAG_NormalTurret.generated.h"

class UParticleSystem;
class USoundBase;
class USceneComponent;
class USkeletalMeshComponent;

UCLASS()
class OFPLANETSANDGUNS_API AOPAG_NormalTurret : public AOPAG_Enemy
{
	GENERATED_BODY()

public:

	AOPAG_NormalTurret();

	virtual void Attack() override;

	UFUNCTION(CallInEditor, Category = "Normal Turret")
		virtual void Death() override;

	UFUNCTION(BlueprintCallable, Category = "Normal Turret")
		FRotator GetTurretHeadRotation() const;

	virtual void Tick(float DeltaSeconds) override;

protected:	

	FVector GetRandomImpulseVector() const;

	//TODO: This breaks everything
	UPROPERTY(EditAnywhere, Category="Normal Turret")
	float ExplosionImpulseRandomRange = 900.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Normal Turret")
		USceneComponent* Muzzle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Normal Turret")
		UParticleSystem* MuzzleParticle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Normal Turret")
		UParticleSystem* HitParticle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Normal Turret")
		UParticleSystem* DeathParticle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Normal Turret")
		USoundBase* ShootSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Normal Turret")
		FRotator TurretHeadRotation;	
};
