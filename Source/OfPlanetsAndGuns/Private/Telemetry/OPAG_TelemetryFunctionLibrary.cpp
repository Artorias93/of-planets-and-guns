// Fill out your copyright notice in the Description page of Project Settings.


#include "Telemetry/OPAG_TelemetryFunctionLibrary.h"

FProperty* UOPAG_TelemetryFunctionLibrary::RetrieveProperty(AActor* InActor, const FString& InPath, void*& OutTarget)
{

	//first param is already an Actor instead of a UObject
	//(since this is gonna be used on the player primarily it doesn't need to be generalized to a UObject)

	ensureAlways(InActor);

	if (InActor == nullptr) return nullptr;

	FString ObjectString;
	FString PropertyString;

	bool bSucceded = InPath.Split(".", &ObjectString, &PropertyString);

	ensureAlways(bSucceded);

	UObject* TargetObject = nullptr;
	if (ObjectString.Equals("this", ESearchCase::IgnoreCase))
	{
		TargetObject = InActor;
	}
	else
	{
		TArray<UActorComponent*> Components;
		InActor->GetComponents(Components);

		for (UActorComponent* Component : Components)
		{
			if (ObjectString.Equals(Component->GetName(), ESearchCase::IgnoreCase))
			{
				TargetObject = Component;
				break;
			}
		}
	}

	OutTarget = TargetObject;

	FProperty* OutProperty;

	if (ensureAlways(TargetObject))
	{
		UClass* TargetClass = TargetObject->GetClass();
		FName PropertyName = FName(*PropertyString);

		OutProperty = TargetClass->FindPropertyByName(PropertyName);
	}
	else
	{
		OutProperty = nullptr;
	}


	return OutProperty;
}
