// Fill out your copyright notice in the Description page of Project Settings.


#include "InputFunctions.h"
#include <OfPlanetsAndGuns/Public/Character/CharacterBase.h>
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>


UInputMappingContext* UInputFunctions::GetCurrentMappingContext(UPARAM(Ref) ACharacterBase* Character)
{
	if (Character == nullptr) return nullptr;
	return Character->MappingContext;
}

void UInputFunctions::GetInputActionKey(UPARAM(Ref)UInputAction* InputAction, UPARAM(Ref)UInputMappingContext* MappingContext, TArray<FEnhancedActionKeyMapping>& Keys)
{
	if (MappingContext == nullptr) return;
	auto mappings = MappingContext->GetMappings();
	for (int32 i = 0; i < mappings.Num(); ++i)
	{
		if (InputAction == mappings[i].Action)
		{
			Keys.Add(mappings[i]);
		}
	}
}

void UInputFunctions::UnmapKeyAndPrepareForRemap(UPARAM(Ref) FKey NewKey, UPARAM(Ref) UInputAction* InputAction, UPARAM(Ref) UInputMappingContext* MappingContext, int32 KeyIndex, int32& IndexInMapping, FEnhancedActionKeyMapping& NewMapping)
{
	TArray<FEnhancedActionKeyMapping> Keys;
	GetInputActionKey(InputAction, MappingContext, Keys);
	if (Keys.Num() <= KeyIndex) return;
	if (Keys[KeyIndex].Key.IsGamepadKey() != NewKey.IsGamepadKey()) return;


	
	if (!MappingContext->GetMappings().Find(Keys[KeyIndex], IndexInMapping)) return;

	NewMapping = Keys[KeyIndex];
	NewMapping.Key = NewKey;

	MappingContext->UnmapKey(InputAction, Keys[KeyIndex].Key);

	/*auto mappings = MappingContext->GetMappings();
	mappings.Insert(newMapping, indexInMapping);
	UEnhancedInputLibrary::RequestRebuildControlMappingsUsingContext(MappingContext, true);*/
}
