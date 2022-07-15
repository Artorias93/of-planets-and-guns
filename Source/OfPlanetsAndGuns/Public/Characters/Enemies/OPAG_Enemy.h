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

	void UpdateSpeed(float NewSpeed);

	virtual void TakeDamage(float DamageAmount) override;

	virtual void Attack();

	virtual void Death();

	virtual void Tick(float DeltaSeconds) override;

protected:

	virtual void BeginPlay() override;

	void DropAndDespawn();

	/////////////////////////////
	//Variables
	/////////////////////////////

public:

	UPROPERTY(EditAnywhere, Category = "OPAG AI")
	class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OPAG AI", meta = (ExposeOnSpawn))
	class AOPAG_PatrolPath* PatrolPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OPAG AI", meta = (ExposeOnSpawn))
	bool bUsingPatrolPath = false;

	UPROPERTY(EditAnywhere, Category = "OPAG AI")
	float RagdollDurationSeconds = 2.f;

protected:

	FTimerHandle RagdollTimerHandle;
};
