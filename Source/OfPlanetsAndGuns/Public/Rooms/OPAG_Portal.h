// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactables/OPAG_InteractableInterface.h"
#include "Managers/StageManager/OPAG_StageGeneration.h"
#include "OPAG_Portal.generated.h"


UCLASS()
class OFPLANETSANDGUNS_API AOPAG_Portal : public AActor, public IOPAG_InteractableInterface
{
	GENERATED_BODY()

	uint8 bIsOpen : 1;

public:	
	// Sets default values for this actor's properties
	AOPAG_Portal();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Portal, Meta=(ExposeOnSpawn = true))
	EStageType PortalType;


private:


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	static AOPAG_Portal* SpawnPortal(UWorld* World, TSubclassOf<AOPAG_Portal> Class, const EStageType Type,
		const FVector& Location, const FRotator& Rotation);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void UnlockPortal();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void Interact();
	virtual void Interact_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		bool CanInteract();
	virtual bool CanInteract_Implementation() override;


};
