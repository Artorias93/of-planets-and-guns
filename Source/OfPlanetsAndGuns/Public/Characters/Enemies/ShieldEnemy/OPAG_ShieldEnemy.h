#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemies/MeleeEnemy/OPAG_MeleeEnemy.h"
#include "OPAG_ShieldEnemy.generated.h"

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

	UStaticMeshComponent* GetShieldMesh();

protected:

	virtual void BeginPlay() override;

private:	

	UFUNCTION()
	void OnAttackBegin(UAnimMontage* Montage);

	UFUNCTION()
	void OnAttackNotify(FName FunctionName, const FBranchingPointNotifyPayload& NotifyPayload);

	UFUNCTION()
	void OnAttackEnd(UAnimMontage* Montage, bool bInterrupted);

	/////////////////////////////
	//Variables
	/////////////////////////////

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

	UPROPERTY(EditAnywhere)
	float ShieldWeaponDamage = 10.f;	
};