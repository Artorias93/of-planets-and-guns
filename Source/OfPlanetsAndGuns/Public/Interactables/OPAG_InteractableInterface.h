// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OPAG_InteractableInterface.generated.h"

/**
 * 
 */
UINTERFACE(BlueprintType, Blueprintable)
class OFPLANETSANDGUNS_API UOPAG_InteractableInterface : public UInterface
{
	GENERATED_BODY()
};

class IOPAG_InteractableInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void Interact();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		bool CanInteract();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void ApplyForce();
};
