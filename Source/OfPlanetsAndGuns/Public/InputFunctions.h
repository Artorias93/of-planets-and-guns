// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "InputMappingContext.h"
#include "EnhancedActionKeyMapping.h"
#include "InputFunctions.generated.h"

/**
 * 
 */
UCLASS()
class OFPLANETSANDGUNS_API UInputFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static UInputMappingContext* GetCurrentMappingContext(UPARAM(Ref) ACharacterBase* Character);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static void GetInputActionKey(UPARAM(Ref) UInputAction* InputAction, UPARAM(Ref) UInputMappingContext* MappingContext, TArray<FEnhancedActionKeyMapping>& Keys);

	UFUNCTION(BlueprintCallable)
	static void UnmapKeyAndPrepareForRemap(UPARAM(Ref) FKey NewKey, UPARAM(Ref) UInputAction* InputAction, UPARAM(Ref) UInputMappingContext* MappingContext, int32 KeyIndex, int32& IndexInMapping, FEnhancedActionKeyMapping& NewMapping);
};
