// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemies/OPAG_Enemy.h"
#include <../../../FX/Niagara/Source/Niagara/Public/NiagaraComponent.h>
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
		UNiagaraComponent* GetLaserBeam() const;

	void StartShooting();
	
	void GetShootingDirection(FVector& Start, FVector& ShootDirection, float const TurretShootingAccuracyTolerance);

	void ShootLineTrace(FVector StartShootLocation, FVector Direction);

	TArray<AActor*> GetAllActorsToIgnore();

protected:	

	FVector GetRandomImpulseVector() const;

	UPROPERTY(EditDefaultsOnly, Category = "Normal Turret")
		float ExplosionPower = 900.f;

	UPROPERTY(EditDefaultsOnly, Category = "Normal Turret")
	float ShootingTimer = .5f;

	UPROPERTY(EditAnywhere, Category = "Normal Turret")
	float RandomAngleShotBounds = 2.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Normal Turret")
		float Damage;

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

	UPROPERTY(VisibleAnywhere)
		UNiagaraComponent* LaserBeam;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Normal Turret")
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesQuery;

	FTimerHandle ShotTimerHandle;

	UPROPERTY()
		FVector StartLocation = FVector::ZeroVector;
	UPROPERTY()
		FVector TurretShootDirection = FVector::ZeroVector;
};
