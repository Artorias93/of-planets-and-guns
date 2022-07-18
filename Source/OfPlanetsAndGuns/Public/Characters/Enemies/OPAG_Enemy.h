#pragma once

#include "CoreMinimal.h"
#include "Characters/OPAG_CharacterBase.h"
#include "OPAG_Enemy.generated.h"

UCLASS()
class OFPLANETSANDGUNS_API AOPAG_Enemy : public AOPAG_CharacterBase
{
	GENERATED_BODY()

	/////////////////////////////
	//Functions
	/////////////////////////////

public:

	AOPAG_Enemy();

	void UpdateSpeed(float const NewSpeed) const;
	
	virtual void ReceiveDamage(float DamageAmount) override;

	//TODO: Remove when the enemy takes damage normally
	UFUNCTION(CallInEditor, Category = "OPAG AI")
	void DebugTake100Damage();

	virtual void Attack();

	virtual void Death() override;	

protected:

	virtual void BeginPlay() override;

	void DropAndDespawn();

	virtual void GetShootingDirection(FVector& StartLocation, FVector& Direction, float const ShootingAccuracyTolerance /* = 0 */) override;
	
	/////////////////////////////
	//Variables
	/////////////////////////////

public:

	UPROPERTY(EditAnywhere, Category = "OPAG AI")
	class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OPAG AI")
	class AOPAG_PatrolPath* PatrolPath;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OPAG AI", meta = (ExposeOnSpawn, AllowPrivateAccess = true))
	bool bUsingPatrolPath = false;

	UPROPERTY()
	FTimerHandle RagdollTimerHandle;

	UPROPERTY(EditAnywhere, Category = "OPAG AI", meta=(AllowPrivateAccess = true))
	float RagdollDurationSeconds = 2.f;
};
