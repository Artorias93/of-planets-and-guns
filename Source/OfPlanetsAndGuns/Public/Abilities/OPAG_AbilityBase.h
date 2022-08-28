// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OPAG_AbilityBase.generated.h"

UCLASS()
class OFPLANETSANDGUNS_API AOPAG_AbilityBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AOPAG_AbilityBase();

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
		class UOPAG_EventManagerSubsystem* EMS;

	UPROPERTY(EditAnywhere, Category = "Mesh")
		class UStaticMeshComponent* AbilityMesh;
};
