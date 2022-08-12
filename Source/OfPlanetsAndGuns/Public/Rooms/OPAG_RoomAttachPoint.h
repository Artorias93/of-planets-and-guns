// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OPAG_RoomAttachPoint.generated.h"

class UArrowComponent;
class UBillboardComponent;
class AOPAG_RoomManager;
UCLASS()
class OFPLANETSANDGUNS_API AOPAG_RoomAttachPoint : public AActor
{
	GENERATED_BODY()

	UTexture2D* StartSprite;
	UTexture2D* ExitSprite;
	
public:	
	// Sets default values for this actor's properties
	AOPAG_RoomAttachPoint();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBillboardComponent* Billboard;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UArrowComponent* Arrow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsStart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FTransform RoomOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsOpen;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void UpdateAttachPointData(AOPAG_RoomManager* const RoomManager);

};
