#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Logging/TokenizedMessage.h"
#include "OPAG_UtilityFunctionLibrary.generated.h"

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


	template<typename T>
	static FORCEINLINE void MapBaseItemsWithDifficulty(TMap<TSubclassOf<T>, float>& Items,
		float Difficulty, float Spread = 1.f)
	{
		float OverallProbability = 0;

		for (const auto Item : Items)
		{
			float Probability = GaussianProbabilityModifier(Difficulty, Spread, Item.Value);
			OverallProbability += Probability;
			Items[Item.Key] = Probability;
		}

		for (const auto Item : Items)
		{
			Items[Item.Key] /= OverallProbability;
		}
	}

	template<typename T>
	static FORCEINLINE TSubclassOf<T> GetRandomItem(TMap<TSubclassOf<T>, float>& WeightedMap)
	{
		const float RandomChance = FMath::FRandRange(0.f, 1.f);
		float Sum = 0.f;
		TArray<TSubclassOf<T>> Keys;
		WeightedMap.GetKeys(Keys);
		TSubclassOf<T> ChosenItem = Keys.Last();
		for (TSubclassOf<T> Key : Keys)
		{
			Sum += WeightedMap[Key];
			if (RandomChance < Sum)
			{
				ChosenItem = Key;
				break;
			}
		}
		return ChosenItem;
	}

private:
	static FORCEINLINE float GaussianProbabilityModifier(float Difficulty, float Spread, float Level)
	{
		float ModifiedLevel = Difficulty - Level;
		ModifiedLevel /= Spread;
		return FMath::Exp(FMath::Pow(ModifiedLevel, 2.f) * -0.5f);
	}
};
