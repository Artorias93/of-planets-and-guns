// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/OPAG_AbilityBase.h"
#include "OPAG_ForceField.generated.h"

UCLASS()
class OFPLANETSANDGUNS_API AOPAG_ForceField : public AOPAG_AbilityBase
{
	GENERATED_BODY()
	
public:	
	AOPAG_ForceField();	
	virtual void Init(EAbilityTypes AbilityType, float Delay) override;

	UFUNCTION()
		void GetHit(AActor* OwnerInstance);

protected:
	virtual void BeginPlay() override;		

	UPROPERTY(EditAnywhere, Category = "Collider")
		class USphereComponent* AbilityCollider;

	UPROPERTY(EditAnywhere, Category = "Ability")
		class UStaticMeshComponent* AbilityMesh;

	UPROPERTY(EditAnywhere, Category = "Ability")
		class UMaterial* ForceFieldMaterial;

	UPROPERTY(EditAnywhere, Category = "Ability")
		class UMaterial* ForceFieldHitMaterial;

	UFUNCTION()
		void OnPillarDestroyed();

	UFUNCTION()
		virtual void DestroyAbility() override;

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	int AvailablePillarsAmount = 0;

	TArray<AActor*> Pillars;

	void ResetMaterial();
};
