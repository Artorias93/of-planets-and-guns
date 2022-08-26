#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OPAG_PatrolPath.generated.h"

UCLASS()
class OFPLANETSANDGUNS_API AOPAG_PatrolPath : public AActor
{
	GENERATED_BODY()

	/////////////////////////////
	//Functions
	/////////////////////////////
	
public:

	AOPAG_PatrolPath();

	/////////////////////////////
	//Variables
	/////////////////////////////

public:

	UPROPERTY(EditAnywhere, Category = "AI", Meta = (MakeEditWidget = true))
	TArray<FVector> PatrolPathPoints;

//private:
	
	//UPROPERTY(EditAnywhere, Category = "AI", meta=(AllowPrivateAccess = true))
	//int StartingPatrolPathIndex; //Currently unused
};