#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemies/MeleeEnemy/OPAG_MeleeEnemy.h"
#include "OPAG_ShieldEnemy.generated.h"

class UTimelineComponent;

UCLASS()
class OFPLANETSANDGUNS_API AOPAG_ShieldEnemy : public AOPAG_Enemy
{
	GENERATED_BODY()

	/////////////////////////////
	//Functions
	/////////////////////////////

public:	

	AOPAG_ShieldEnemy();	
	
	virtual void Attack() override;

	virtual void Death() override;

	UFUNCTION(BlueprintCallable)
	bool GetIsCharging() const;
	
	UFUNCTION(BlueprintCallable)
	void SetIsCharging(const bool Value);
	
	UStaticMeshComponent* GetShieldMesh() const;
	
	void TurnOnBoosters() const;
	
	void TurnOffBoosters() const;

protected:

	virtual void BeginPlay() override;

private:	

	UFUNCTION()
	void OnAttackBegin(UAnimMontage* Montage);

	UFUNCTION()
	void OnAttackNotify(FName FunctionName, const FBranchingPointNotifyPayload& NotifyPayload);

	UFUNCTION()
	void OnAttackEnd(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//void TimelineCallback(const float Height) const;

	/////////////////////////////
	//Variables
	/////////////////////////////
	
//public:

	// UPROPERTY(EditDefaultsOnly, Category = "OPAG AI")
	// UCurveFloat* MovementCurve;
	
private:

	bool bIsCharging;

	UPROPERTY(EditDefaultsOnly, Category = "OPAG AI")
	UAnimMontage* ShieldAttackMontage;

	UPROPERTY()
	UAnimInstance* AnimInstance;

	UPROPERTY(EditDefaultsOnly, Category = "OPAG AI")
	float ShieldAttackMontagePlayRate = 1.2f;

	UPROPERTY(EditAnywhere, Category = "OPAG AI")
	FVector DamageCollisionBoxRelativeLocation = FVector(145.f, 0.f, 7.f);

	UPROPERTY(EditAnywhere, Category = "OPAG AI")
	FVector DamageCollisionBoxExtent = FVector(95.f, 20.f, 90.f);

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* DamageCollisionBox;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* SwordMesh;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ShieldMesh;

	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* ParticleComponent1;

	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* ParticleComponent2;
	
	UPROPERTY(EditAnywhere, Category = "OPAG AI")
	float ShieldWeaponDamage = 10.f;

	UPROPERTY(EditAnywhere, Category = "OPAG AI")
	float ChargeDamage = 100.f;

	// UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	// UTimelineComponent* Timeline;
	//
	// UPROPERTY()
	// FVector InitialLocation;
};