#pragma once

#include "CoreMinimal.h"
#include "Characters/OPAG_CharacterBase.h"
#include "OPAG_Enemy.generated.h"

UCLASS()
class OFPLANETSANDGUNS_API AOPAG_Enemy : public AOPAG_CharacterBase
{
	GENERATED_BODY()

	//Variables
	
public:

	UPROPERTY(EditDefaultsOnly, Category = AI)
	class UBehaviorTree* BehaviorTree;

	//Functions

public:

	void UpdateSpeed(float NewSpeed);

};
