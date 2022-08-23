// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OPAG_ForceField.generated.h"

UCLASS()
class OFPLANETSANDGUNS_API AOPAG_ForceField : public AActor
{
	GENERATED_BODY()
	
public:	
	AOPAG_ForceField();

protected:

	UPROPERTY(EditAnywhere, Category = "Mesh")
		class UStaticMeshComponent* Mesh;
};
