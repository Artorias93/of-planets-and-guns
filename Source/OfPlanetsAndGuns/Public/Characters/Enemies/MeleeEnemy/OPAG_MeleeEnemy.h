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

private:

	void BeginPlay() override;

	UFUNCTION()
	void OnAttackNotify(FName FunctionName, const FBranchingPointNotifyPayload& NotifyPayload);

	/////////////////////////////
	//Variables
	/////////////////////////////

private: 

	UPROPERTY(EditDefaultsOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* MeleeAttackMontage;

	class UBoxComponent* DamageCollisionBoxComponent;

	UPROPERTY(EditAnywhere, Category = "AI", meta = (AllowPrivateAccess = "true"))
	FVector DamageCollisionBoxComponentRelativeLocation = FVector(145.f, 0.f, 7.f);

	UPROPERTY(EditAnywhere, Category = "AI", meta = (AllowPrivateAccess = "true"))
	FVector DamageCollisionBoxComponentExtent = FVector(95.f, 20.f, 90.f);

	UAnimInstance* AnimInstance;

	UPROPERTY(EditDefaultsOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* SwordMeshComponent;
};