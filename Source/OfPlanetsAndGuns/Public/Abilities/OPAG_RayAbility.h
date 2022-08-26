// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/OPAG_AbilityBase.h"
#include "OPAG_RayAbility.generated.h"

UCLASS()
class OFPLANETSANDGUNS_API AOPAG_RayAbility : public AOPAG_AbilityBase
{
	GENERATED_BODY()
	
public:
	AOPAG_RayAbility();
	virtual void Init(EAbilityTypes AbilityType) override;	

protected:
	virtual void BeginPlay() override;

	virtual void DestroyAbility() override;

	UPROPERTY(EditAnywhere, Category = "Mesh")
		class UStaticMeshComponent* AbilityMesh;

	UPROPERTY(EditAnywhere, Category = "Collider")
		class UBoxComponent* AbilityCollider;

	UPROPERTY(EditAnywhere, Category = "Particle Muzzle")
		class UNiagaraComponent* AbilityParticleMuzzle;

	UPROPERTY(EditAnywhere, Category = "Particle Ray")
		class UNiagaraComponent* AbilityParticleRay;

	UPROPERTY(EditAnywhere, Category = "Particle Hit")
		class UNiagaraComponent* AbilityParticleHit;

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
