// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OPAG_Drop.generated.h"

class UStaticMeshComponent;
class USphereComponent;

UENUM(BlueprintType)
enum class EDropType : uint8 {
	Ammo,
	Gold,
	Bolts
};

UCLASS()
class OFPLANETSANDGUNS_API AOPAG_Drop : public AActor
{
	GENERATED_BODY()
	
public:	
	AOPAG_Drop();

	UPROPERTY(Category = "Drop stuff", EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* DropMesh;
	UPROPERTY(Category = "Drop stuff", EditAnywhere, BlueprintReadWrite)
		USphereComponent* Sphere;
	UPROPERTY(Category = "Drop stuff", EditDefaultsOnly, BlueprintReadWrite)
		EDropType DropType;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
};
