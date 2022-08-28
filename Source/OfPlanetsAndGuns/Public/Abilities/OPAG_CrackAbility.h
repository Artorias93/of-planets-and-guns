// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/OPAG_AbilityBase.h"
#include "OPAG_CrackAbility.generated.h"

/**
 * 
 */
UCLASS()
class OFPLANETSANDGUNS_API AOPAG_CrackAbility : public AOPAG_AbilityBase
{
	GENERATED_BODY()

	float DamageTimer = 0;
	bool bDealDamage = true;
	class AOPAG_Player* OverlappingTarget = nullptr;

public:
	AOPAG_CrackAbility();
	virtual void Init(EAbilityTypes AbilityType, float Delay) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
		float DamageOverTimeFrequency;

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Mesh")
		class UStaticMeshComponent* AbilityMesh;

	UPROPERTY(EditAnywhere, Category = "Collider")
		class UBoxComponent* AbilityCollider;

	UPROPERTY(EditAnywhere, Category = "Particle")
		class UNiagaraComponent* AbilityParticle;

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		virtual void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void DestroyAbility() override;

	void DealDamage(AActor* TargetActor);
};
