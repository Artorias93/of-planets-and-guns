// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OPAG_LootSpawnPoint.generated.h"

class UBillboardComponent;
UCLASS()
class OFPLANETSANDGUNS_API AOPAG_LootSpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOPAG_LootSpawnPoint();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBillboardComponent* Billboard;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
