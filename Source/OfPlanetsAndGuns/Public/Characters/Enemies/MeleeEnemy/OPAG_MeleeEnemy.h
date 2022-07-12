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

protected:

	virtual void BeginPlay() override;

private:

	float PlayMeleeAttackMontage();

	/////////////////////////////
	//Variables
	/////////////////////////////

private: 

	UPROPERTY(EditDefaultsOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* MeleeAttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* SwordMeshComponent;
};