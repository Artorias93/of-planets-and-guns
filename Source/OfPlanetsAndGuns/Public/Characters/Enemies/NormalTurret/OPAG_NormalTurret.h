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
		FRotator GetTurretHeadRotation() const;

	UFUNCTION(BlueprintCallable, Category = "Normal Turret")
		UNiagaraComponent* GetLaserBeam() const;

	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;

	void StartShooting();
	
	void GetShootingDirection(FVector& Start, FVector& ShootDirection, float const ShootingAccuracyTolerance);

	void ShootLineTrace(FVector StartShootLocation, FVector Direction);

	TArray<AActor*> GetAllActorsToIgnore();

protected:	

	FVector GetRandomImpulseVector() const;

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
