#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemies/OPAG_Enemy.h"
#include "OPAG_MeleeEnemy.generated.h"

UCLASS()
class OFPLANETSANDGUNS_API AOPAG_MeleeEnemy : public AOPAG_Enemy
{
	GENERATED_BODY()	

	/////////////////////////////
	//Functions
	/////////////////////////////

public:

	AOPAG_MeleeEnemy();

	void Attack() override;

	UFUNCTION(CallInEditor, Category = "OPAG AI")
	void Death() override;

	
private:

	void BeginPlay() override;

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

	UPROPERTY(EditDefaultsOnly, Category = "OPAG AI")
	class UAnimMontage* MeleeAttackMontage;

	UAnimInstance* AnimInstance;

	UPROPERTY(EditDefaultsOnly, Category = "OPAG AI")
	float MeleeAttackMontagePlayRate = 1.9f;

	UPROPERTY(EditAnywhere, Category = "OPAG AI")
	FVector DamageCollisionBoxRelativeLocation = FVector(145.f, 0.f, 7.f);

	UPROPERTY(EditAnywhere, Category = "OPAG AI")
	FVector DamageCollisionBoxExtent = FVector(95.f, 20.f, 90.f);

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* DamageCollisionBox;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* SwordMesh;
};