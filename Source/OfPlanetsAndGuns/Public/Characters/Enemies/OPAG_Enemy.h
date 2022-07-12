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

	void TakeDamage(float DamageAmount);

	void Attack();

	void Death();

	virtual void Tick(float DeltaSeconds) override;

protected:

	virtual void BeginPlay() override;
	
private:

	void DropAndDespawn();

	/////////////////////////////
	//Variables
	/////////////////////////////

public:

	UPROPERTY(EditAnywhere, Category = "AI")
	class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (ExposeOnSpawn))
	class AOPAG_PatrolPath* PatrolPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite ,Category = "AI", meta = (ExposeOnSpawn))
	bool bUsingPatrolPath;

	UPROPERTY(EditAnywhere, Category = "AI")
	float RagdollDurationSeconds;

private:

	FTimerHandle RagdollTimerHandle;
};
