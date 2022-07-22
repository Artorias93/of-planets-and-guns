// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Logging/TokenizedMessage.h"
#include "OPAG_UtilityFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class OFPLANETSANDGUNS_API UOPAG_UtilityFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:


	static void ScreenLogWithSeverityColor(const UObject* WorldContextObject, 
		const FString& Message, EMessageSeverity::Type Severity);

	template<typename T>
	static FORCEINLINE void ShuffleArray(TArray<T>& Array)
	{
		if (Array.Num() > 1)
		{
			const int32 LastIndex = Array.Num() - 1;
			for (int32 i = 0; i <= LastIndex; ++i)
			{
				int32 Index = FMath::RandRange(i, LastIndex);
				if (i != Index)
				{
					Array.Swap(i, Index);
				}
			}
		}
	}

};
