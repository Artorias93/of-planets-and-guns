// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OPAG_DoorManagerComponent.generated.h"


class AOPAG_Portal;
class AOPAG_DoorBase;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OFPLANETSANDGUNS_API UOPAG_DoorManagerComponent : public UActorComponent
{
	GENERATED_BODY()

	AOPAG_Portal* ExitPortal;

public:	
	// Sets default values for this component's properties
	UOPAG_DoorManagerComponent();

	UPROPERTY(Category = "Door Manager", EditAnywhere, BlueprintReadWrite)
	TArray<AOPAG_DoorBase*> Doors;

	UPROPERTY(Category = "Door Manager", EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AOPAG_Portal> PortalClass;
	UPROPERTY(Category = "Door Manager", EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AActor> ExitWallClass;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetExitAsSpecial(bool bIsLastRoom, bool bHasSafeRoom, const FVector Location, const FRotator Rotation);
	void BlockExit(const FVector Location, const FRotator Rotation) const;

	UFUNCTION()
		void CloseDoors();

	UFUNCTION()
		void OpenDoors();
};
