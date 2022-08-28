// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "OPAG_DamageDealingFloor.generated.h"

class AOPAG_CharacterBase;
class UBoxComponent;
UCLASS()
class OFPLANETSANDGUNS_API AOPAG_DamageDealingFloor : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
		UBoxComponent* Floor;
	UPROPERTY(VisibleAnywhere)
		UBoxComponent* FloorVolume;

	AOPAG_CharacterBase* CharacterToDamage = nullptr;

	UTimelineComponent* TickTimeline;

	FOnTimelineEventStatic OnTimelineFinished;
public:	
	// Sets default values for this actor's properties
	AOPAG_DamageDealingFloor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DamagePerTick;

	//Tick time is in seconds
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float TickTime;

private:
	UFUNCTION()
		void TimelineFinished();

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
