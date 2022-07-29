// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "OPAG_ZiplineSupport.generated.h"


UCLASS()
class OFPLANETSANDGUNS_API AOPAG_ZiplineSupport : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AOPAG_ZiplineSupport();

	UPROPERTY(EditDefaultsOnly, BLueprintReadWrite, Category = "ZiplineSupport")
		USceneComponent* Root;

	UPROPERTY(EditDefaultsOnly, BLueprintReadWrite, Category = "ZiplineSupport | Mesh")
		UStaticMeshComponent* SupportMesh;

	UPROPERTY(EditAnyWhere, BLueprintReadWrite, Category = "ZiplineSupport | Collider")
		UBoxComponent* InteractableArea;

	UPROPERTY(EditAnyWhere, BLueprintReadWrite, Category = "ZiplineSupport | AttachPoint")
		USceneComponent* CableAttachPoint;

	UPROPERTY(EditDefaultsOnly, BLueprintReadWrite, Category = "ZiplineSupport | AttachPoint")
		USceneComponent* PlayerAttachPoint;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
