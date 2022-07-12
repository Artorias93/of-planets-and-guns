// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Volume.h"
#include "OPAG_RoomStreamingVolume.generated.h"

class UOPAG_EventManagerSubsystem;
class UBoxComponent;
class ULevelStreamingDynamic;
/**
 * 
 */
UCLASS()
class OFPLANETSANDGUNS_API AOPAG_RoomStreamingVolume : public AActor
{
	GENERATED_BODY()

		UOPAG_EventManagerSubsystem* EventManager;

public:
	// Sets default values for this actor's properties
	AOPAG_RoomStreamingVolume();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UBoxComponent* Volume;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<ULevelStreamingDynamic*> StreamingLevels;

	//AOPAG_StageManager make an event

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
			int32 OtherBodyIndex);
};
