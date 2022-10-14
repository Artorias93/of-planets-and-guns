// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OPAG_PlayerTelemetryComponent.generated.h"

USTRUCT(BlueprintType)
struct OFPLANETSANDGUNS_API FOPAG_TelemetryQuery
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FString Component;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FString Query;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OFPLANETSANDGUNS_API UOPAG_PlayerTelemetryComponent : public UActorComponent
{
	GENERATED_BODY()

	FString CurrentTelemetryList;
	FString CurrentTelemetryPreset;

	TArray<FString>	TelemetryArray;

	struct FTelemetryTarget
	{
		void* TargetObject;
		FProperty* TargetProperty;
	};

	TArray<FTelemetryTarget> TelemetryTargets;


public:	
	// Sets default values for this component's properties
	UOPAG_PlayerTelemetryComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Telemetry Presets")
		TArray<FOPAG_TelemetryQuery> TelemetryQueriesPreset1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Telemetry Presets")
		TArray<FOPAG_TelemetryQuery> TelemetryQueriesPreset2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Telemetry Presets")
		TArray<FOPAG_TelemetryQuery> TelemetryQueriesPreset3;



private:
	void UpdateTelemetryTargets();

	

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void DrawTelemetry(UCanvas* InCanvas, float& InOutYL, float& InOutYPos);
};
