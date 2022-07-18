// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Environment/OPAG_DoorBase.h"
#include "OPAG_SimpleDoor.generated.h"

class UTimelineComponent;
class UBoxComponent;
/**
 * 
 */
UCLASS()
class OFPLANETSANDGUNS_API AOPAG_SimpleDoor : public AOPAG_DoorBase
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* Frame;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* Door;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UBoxComponent* CollisionVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UTimelineComponent* Timeline;

	FVector InitialDoorLocation;
	uint8 bIsOpen : 1;

public:

	AOPAG_SimpleDoor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UCurveFloat* MovementCurve;

private:

	UPROPERTY()
		float TimelineFloatProperty;

	UFUNCTION()
		void TimelineCallback(float Height) const;

protected:
	virtual void BeginPlay() override;
public:
	virtual void OpenDoor_Implementation() override;
	virtual void CloseDoor_Implementation() override;


};
