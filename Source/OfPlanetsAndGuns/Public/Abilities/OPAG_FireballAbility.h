// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/OPAG_AbilityBase.h"
#include "OPAG_FireballAbility.generated.h"

/**
 * 
 */
UCLASS()
class OFPLANETSANDGUNS_API AOPAG_FireballAbility : public AOPAG_AbilityBase
{
	GENERATED_BODY()
	bool bShoot = false;
	FVector TargetLocation; 
public:
	AOPAG_FireballAbility();
	virtual void Init(EAbilityTypes AbilityType, float Delay) override;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	float Speed;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void DestroyAbility() override;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	class UStaticMeshComponent* AbilityMesh;

	UPROPERTY(EditAnywhere, Category = "Collider")
	class UBoxComponent* AbilityCollider;

	UPROPERTY(EditAnywhere, Category = "Particle Muzzle")
	class UParticleSystemComponent* AbilityParticleMuzzle;

	UPROPERTY(EditAnywhere, Category = "Particle Projectile")
	class UParticleSystem* AbilityParticleProjectile;

	UPROPERTY(EditAnywhere, Category = "Particle Hit")
	class UParticleSystem* AbilityParticleHit;	

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void ShootProjectile();
};
