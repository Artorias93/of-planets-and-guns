// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OPAG_DoorBase.generated.h"

UCLASS()
class OFPLANETSANDGUNS_API AOPAG_DoorBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOPAG_DoorBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent)
		void OpenDoor(); virtual void OpenDoor_Implementation();
	UFUNCTION(BlueprintNativeEvent)
		void CloseDoor(); virtual void CloseDoor_Implementation();
};
