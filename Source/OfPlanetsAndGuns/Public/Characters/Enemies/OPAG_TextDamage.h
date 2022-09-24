// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OPAG_TextDamage.generated.h"

UCLASS()
class OFPLANETSANDGUNS_API AOPAG_TextDamage : public AActor
{
	GENERATED_BODY()
	
public:	

	UPROPERTY(EditDefaultsOnly)
	class UWidgetComponent* WidgetComponent;

	UPROPERTY(EditDefaultsOnly)
		FVector InitialPosition;

	UPROPERTY(EditDefaultsOnly)
		FVector FinalPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ZOffsetRange = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float YOffsetRange = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float IncomingDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bCritic;

	AOPAG_TextDamage();
	void Init(float Damage, bool bCrit);
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	void RemoveFromScene();
};
