// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OPAG_DoorEntranceVolume.generated.h"

class AOPAG_RoomManager;
class UBoxComponent;
UCLASS()
class OFPLANETSANDGUNS_API AOPAG_DoorEntranceVolume : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* Volume;

public:	
	// Sets default values for this actor's properties
	AOPAG_DoorEntranceVolume();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Room Manager")
	AOPAG_RoomManager* RoomManager;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
