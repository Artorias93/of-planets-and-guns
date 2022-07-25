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

	virtual void Attack();

	virtual void Death() override;	

protected:

	virtual void BeginPlay() override;

	void DropAndDespawn();

	virtual void GetShootingDirection(FVector& StartLocation, FVector& Direction, float const ShootingAccuracyTolerance /* = 0 */) override;
	
	/////////////////////////////
	//Variables
	/////////////////////////////

protected:

	UPROPERTY(EditAnywhere, Category = "OPAG AI", meta=(AllowPrivateAccess = true))
	class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OPAG AI", meta=(AllowPrivateAccess = true))
	class AOPAG_PatrolPath* PatrolPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OPAG AI", meta = (ExposeOnSpawn, AllowPrivateAccess = true))
	bool bUsingPatrolPath = false;

	UPROPERTY()
	FTimerHandle RagdollTimerHandle;

	UPROPERTY(EditAnywhere, Category = "OPAG AI", meta=(AllowPrivateAccess = true))
	float RagdollDurationSeconds = 2.f;
};
